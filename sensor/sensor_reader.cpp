// sensor_reader.cpp
// Simple simulated sensor reader; outputs a single JSON line to stdout.

#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

#include "sensor.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
    // Optionally take one argument: number of readings to produce (default 1)
    int count = 1;
    if (argc >= 2) {
        try { count = std::stoi(argv[1]); }
        catch(...) { count = 1; }
    }

    for (int i = 0; i < count; ++i) {
        sensor s;
        auto sensor_data = s.transmit_data();

        json j;
        j["temperature_c"] = sensor_data.temperature_c;
        j["humidity"] = sensor_data.humidity;
        j["timestamp"] = sensor_data.timestamp;
        std::cout << j.dump() << std::endl;
        // If producing multiple readings, sleep a bit
        if (i < count - 1) std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}
