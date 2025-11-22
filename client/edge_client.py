import subprocess
import json
import time
import requests
import sys
from joblib import load
import numpy as np
from pathlib import Path

MODEL_PATH = str(Path(__file__).resolve().parents[1] / "ai" / "models" / "model.joblib")
SENSOR_BIN = str(Path(__file__).resolve().parents[1] / "sensor" / "sensor_reader")
SERVER_URL = "http://192.168.0.100:5000/ingest"

def read_sensor():
    # call sensor_reader and parse JSON stdout
    try:
        out = subprocess.check_output([SENSOR_BIN, "1"], universal_newlines=True, timeout=5)
        # sensor_reader prints one JSON per line; take first non-empty line
        for line in out.splitlines():
            line = line.strip()
            if line:
                return json.loads(line)
    except subprocess.CalledProcessError as e:
        print("Sensor read failed:", e, file=sys.stderr)
    except Exception as e:
        print("Sensor read error:", e, file=sys.stderr)
    return None

def load_model():
    model = load(MODEL_PATH)
    return model

def infer(model, reading):
    X = np.array([[reading["temperature_c"], reading["humidity"]]])
    is_anom = model.predict(X)  # isolation forest: -1 anomaly, 1 normal
    score = model.decision_function(X)[0]  # larger = more normal
    return int(is_anom[0] == -1), float(score)

def send(server_url, payload):
    try:
        r = requests.post(server_url, json=payload, timeout=3)
        return r.status_code, r.text
    except Exception as e:
        print("Send failed:", e)
        return None, str(e)

def main(poll_interval=5):
    model = load_model()
    print("Loaded model:", MODEL_PATH)
    while True:
        reading = read_sensor()
        if reading is None:
            time.sleep(poll_interval)
            continue
        is_anom, score = infer(model, reading)
        payload = {
            "temperature_c": reading["temperature_c"],
            "humidity": reading["humidity"],
            "timestamp": reading["timestamp"],
            "anomaly": bool(is_anom),
            "score": score,
            "node_id": "raspi-01"
        }
        print("Reading:", payload)
        status, text = send(SERVER_URL, payload)
        print("POST ->", SERVER_URL, "status:", status)
        time.sleep(poll_interval)

if __name__ == "__main__":
    if len(sys.argv) >= 2:
        SERVER_URL = sys.argv[1]
    main()
