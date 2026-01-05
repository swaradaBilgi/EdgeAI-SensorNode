# EdgeAI-SensorNode
Training an AI model to find anomolies in received sensor data.

Demo project: C++ sensor reader (simulated), Python model training,
edge client which performs inference and posts results to a Flask server.

## Structure:
- `sensor/` - C++ sensor reader (simulated). To be replaced with real sensor code when ready.
- `ai/` - training script and saved model.
- `client/` - edge client that calls sensor binary, runs inference, and sends to server.
- `server/` - Flask dashboard that receives readings.

## How to run:
1. Install dependencies: `sudo apt install build-essential python3-pip`
2. Setup python venv: `python3 -m venv venv && source venv/bin/activate`
3. Install python libs: `pip install numpy scikit-learn joblib flask requests`
4. Build sensor reader: `cd sensor && g++ -std=c++17 sensor_reader.cpp -o sensor_reader`
5. Train the model: `cd ai && python3 train_model.py`
6. Run server: `cd server && python3 server.py`
7. Run client: `cd client && python3 edge_client.py http://<server-ip>:5000/ingest`
