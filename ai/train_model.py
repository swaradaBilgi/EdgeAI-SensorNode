# train_model.py
import numpy as np
from sklearn.ensemble import IsolationForest
from joblib import dump
import os

def generate_synthetic_data(n=2000, seed=42):
    rng = np.random.RandomState(seed)
    # Normal data around temp=24 +-1, humidity=50 +- 5
    temps = rng.normal(loc=24.0, scale=0.8, size=n)
    hums = rng.normal(loc=50.0, scale=3.0, size=n)
    data = np.vstack([temps, hums]).T
    return data

def main():
    X = generate_synthetic_data()
    model = IsolationForest(contamination=0.01, random_state=0)
    model.fit(X)
    os.makedirs("models", exist_ok=True)
    dump(model, "models/model.joblib")
    print("Model saved to models/model.joblib")

if __name__ == "__main__":
    main()
