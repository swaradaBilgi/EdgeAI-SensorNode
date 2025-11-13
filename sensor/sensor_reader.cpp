// sensor_reader.cpp
// Simple simulated sensor reader; outputs a single JSON line to stdout.
// Replace the read_sensor() implementation with actual sensor code (I2C/SPI/GPIO).

#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp> // we'll use single-header JSON lib - instructions below

using json = nlohmann::json;

struct SensorReading {
    double temperature_c;
    double humidity;
    long timestamp; // unix epoch seconds
};

SensorReading read_sensor() {
    // SIMULATION: random walk around a baseline
    static std::default_random_engine eng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    static std::normal_distribution<double> temp_noise(0.0, 0.3);
    static std::normal_distribution<double> hum_noise(0.0, 0.8);
    static double temp = 24.0;
    static double hum = 50.0;

    temp += temp_noise(eng);
    hum += hum_noise(eng);

    SensorReading r;
    r.temperature_c = std::round(temp * 100.0) / 100.0;
    r.humidity = std::round(hum * 100.0) / 100.0;
    r.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::system_clock::now().time_since_epoch()).count();
    return r;
}

int main(int argc, char** argv) {
    // Optionally take one argument: number of readings to produce (default 1)
    int count = 1;
    if (argc >= 2) {
        try { count = std::stoi(argv[1]); }
        catch(...) { count = 1; }
    }

    for (int i = 0; i < count; ++i) {
        SensorReading r = read_sensor();
        json j;
        j["temperature_c"] = r.temperature_c;
        j["humidity"] = r.humidity;
        j["timestamp"] = r.timestamp;
        std::cout << j.dump() << std::endl;
        // If producing multiple readings, sleep a bit
        if (i < count - 1) std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}
