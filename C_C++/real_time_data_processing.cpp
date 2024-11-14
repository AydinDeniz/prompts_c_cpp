
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>

class SensorSimulator {
public:
    // Simulate reading data from a sensor
    double readData() {
        std::normal_distribution<double> distribution(25.0, 2.0); // Mean of 25, std dev of 2
        return distribution(generator);
    }

private:
    std::default_random_engine generator{std::random_device{}()};
};

class DataProcessor {
public:
    // Apply a simple moving average filter
    double filterData(double newData) {
        dataWindow.push_back(newData);
        if (dataWindow.size() > windowSize) {
            dataWindow.erase(dataWindow.begin());
        }
        
        double sum = 0.0;
        for (double val : dataWindow) {
            sum += val;
        }
        return sum / dataWindow.size();
    }

private:
    std::vector<double> dataWindow;
    const size_t windowSize = 5; // Moving average window size
};

class DataLogger {
public:
    DataLogger(const std::string& filename) : outFile(filename, std::ios::out | std::ios::app) {
        if (!outFile.is_open()) {
            std::cerr << "Failed to open log file." << std::endl;
        }
    }

    // Log processed data to file
    void logData(double data) {
        if (outFile.is_open()) {
            outFile << data << std::endl;
        }
    }

private:
    std::ofstream outFile;
};

int main() {
    SensorSimulator sensor;
    DataProcessor processor;
    DataLogger logger("processed_data.log");

    while (true) {
        double sensorData = sensor.readData();
        double filteredData = processor.filterData(sensorData);
        
        std::cout << "Raw Data: " << sensorData << " | Filtered Data: " << filteredData << std::endl;
        logger.logData(filteredData);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate real-time delay
    }

    return 0;
}
