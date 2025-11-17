from flask import Flask, request, render_template_string
from collections import deque
import time

app = Flask(__name__)

# keep the last 100 readings
LATEST = deque(maxlen=100)

TEMPLATE = """
<!doctype html>
<title>Edge AI Sensor Dashboard</title>
<h1>Latest Readings</h1>
<table border=1 cellpadding=6>
<tr><th>Time</th><th>Node</th><th>Temp (°C)</th><th>Humidity (%)</th><th>Anomaly?</th><th>Score</th></tr>
{% for r in readings %}
<tr>
<td>{{r['time_str']}}</td>
<td>{{r['node_id']}}</td>
<td>{{r['temperature_c']}}</td>
<td>{{r['humidity']}}</td>
<td style="color: {{ 'red' if r['anomaly'] else 'green' }}">{{r['anomaly']}}</td>
<td>{{'%.4f' % r['score']}}</td>
</tr>
{% endfor %}
</table>
"""

@app.route("/ingest", methods=["POST"])
def ingest():
    j = request.get_json(force=True)
    j["time_str"] = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(j.get("timestamp", time.time())))
    LATEST.appendleft(j)
    return "ok", 200

@app.route("/")
def index():
    return render_template_string(TEMPLATE, readings=list(LATEST))

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
