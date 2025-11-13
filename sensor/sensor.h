#include <iostream>
#include <random>
#include <chrono>
#include <thread>

struct SensorReading {
    double temperature_c;
    double humidity;
    long timestamp;
};

class sensor
{
private:
    SensorReading data; 
public:
    SensorReading transmit_data() {
        // SIMULATION: random walk around a baseline
        static std::default_random_engine eng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
        static std::normal_distribution<double> temp_noise(0.0, 0.3);
        static std::normal_distribution<double> hum_noise(0.0, 0.8);
        static double temp = 24.0;
        static double hum = 50.0;

        temp += temp_noise(eng);
        hum += hum_noise(eng);

        
        data.temperature_c = std::round(temp * 100.0) / 100.0;
        data.humidity = std::round(hum * 100.0) / 100.0;
        data.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
        return data;
    }
    
    sensor();
    ~sensor();
};
