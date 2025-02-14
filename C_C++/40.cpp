
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>

class Drone {
public:
    int id;
    double x, y, altitude;
    double speed;
    double direction;
    std::mutex droneMutex;

    Drone(int id, double x, double y, double altitude, double speed)
        : id(id), x(x), y(y), altitude(altitude), speed(speed), direction(0) {}

    void move() {
        std::lock_guard<std::mutex> lock(droneMutex);
        x += speed * cos(direction);
        y += speed * sin(direction);
        altitude += (rand() % 3 - 1) * 0.5;  // Slight altitude variation
    }

    void adjustDirection(double targetX, double targetY) {
        std::lock_guard<std::mutex> lock(droneMutex);
        direction = atan2(targetY - y, targetX - x);
    }

    void displayStatus() {
        std::lock_guard<std::mutex> lock(droneMutex);
        std::cout << "Drone " << id << " -> Position: (" << x << ", " << y << "), Altitude: " << altitude << ", Direction: " << direction << std::endl;
    }
};

class SwarmController {
private:
    std::vector<Drone> drones;
    std::mutex swarmMutex;

public:
    SwarmController(int numDrones) {
        for (int i = 0; i < numDrones; i++) {
            drones.emplace_back(i, rand() % 100, rand() % 100, 50 + rand() % 20, 2 + (rand() % 3));
        }
    }

    void coordinateSwarm(double targetX, double targetY) {
        for (auto &drone : drones) {
            drone.adjustDirection(targetX, targetY);
        }
    }

    void updateSwarm() {
        for (auto &drone : drones) {
            drone.move();
        }
    }

    void displaySwarm() {
        for (auto &drone : drones) {
            drone.displayStatus();
        }
    }

    void runSwarmSimulation(int steps, double targetX, double targetY) {
        for (int i = 0; i < steps; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            coordinateSwarm(targetX, targetY);
            updateSwarm();
            displaySwarm();
            std::cout << "-------------------------" << std::endl;
        }
    }
};

int main() {
    srand(time(NULL));
    SwarmController swarm(5);

    std::thread simulationThread(&SwarmController::runSwarmSimulation, &swarm, 10, 50, 50);
    simulationThread.join();

    return 0;
}
