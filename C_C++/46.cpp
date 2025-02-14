
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>

class Vehicle {
public:
    double x, y, speed, direction;
    std::mutex vehicleMutex;

    Vehicle(double x, double y, double speed, double direction)
        : x(x), y(y), speed(speed), direction(direction) {}

    void move() {
        std::lock_guard<std::mutex> lock(vehicleMutex);
        x += speed * cos(direction);
        y += speed * sin(direction);
    }

    void adjustDirection(double targetX, double targetY) {
        std::lock_guard<std::mutex> lock(vehicleMutex);
        direction = atan2(targetY - y, targetX - x);
    }

    void displayStatus() {
        std::lock_guard<std::mutex> lock(vehicleMutex);
        std::cout << "Vehicle Position: (" << x << ", " << y << "), Speed: " << speed << ", Direction: " << direction << std::endl;
    }
};

class AutonomousVehicleSystem {
private:
    std::vector<Vehicle> vehicles;
    std::mutex systemMutex;

public:
    AutonomousVehicleSystem(int numVehicles) {
        for (int i = 0; i < numVehicles; i++) {
            vehicles.emplace_back(rand() % 100, rand() % 100, 2 + (rand() % 5), 0);
        }
    }

    void detectLanes() {
        std::cout << "[INFO] Detecting lanes...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void avoidObstacles() {
        std::cout << "[INFO] Checking for obstacles...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void coordinateVehicles(double targetX, double targetY) {
        for (auto &vehicle : vehicles) {
            vehicle.adjustDirection(targetX, targetY);
        }
    }

    void updateVehicles() {
        for (auto &vehicle : vehicles) {
            vehicle.move();
        }
    }

    void displayVehicles() {
        for (auto &vehicle : vehicles) {
            vehicle.displayStatus();
        }
    }

    void runSimulation(int steps, double targetX, double targetY) {
        for (int i = 0; i < steps; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            detectLanes();
            avoidObstacles();
            coordinateVehicles(targetX, targetY);
            updateVehicles();
            displayVehicles();
            std::cout << "-------------------------" << std::endl;
        }
    }
};

int main() {
    srand(time(NULL));
    AutonomousVehicleSystem avs(5);

    std::thread simulationThread(&AutonomousVehicleSystem::runSimulation, &avs, 10, 50, 50);
    simulationThread.join();

    return 0;
}
