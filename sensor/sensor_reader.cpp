#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Structure to hold sensor values
struct SensorReading {
    double temperature_c;
    double humidity;
    long timestamp;
};

class Sensor
{
public:
    Sensor() {}
    ~Sensor() {}

    SensorReading transmit_data() {
        SensorReading r;

    double base_temperature = 24.0;
    double base_humidity    = 50.0;

    // Simulate some variation
    double temp_variation = (rand() % 100 - 50) / 100.0;
    double hum_variation  = (rand() % 100 - 50) / 100.0;

    r.temperature_c = base_temperature + temp_variation;
    r.humidity      = base_humidity + hum_variation;

    // Current timestamp
    r.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch()
                  ).count();

    return r;
    }
};

int main() {
    srand(time(NULL));

    // read sensor data
    Sensor s;
    auto r = s.transmit_data();

    // create json from read data
    json j;
    j["temperature_c"] = r.temperature_c;
    j["humidity"] = r.humidity;
    j["timestamp"] = r.timestamp;

    // print output
    std::cout << j.dump() << std::endl;

    return 0;
}
