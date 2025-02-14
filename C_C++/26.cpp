
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Vehicle {
public:
    int position;
    int speed;

    Vehicle(int pos, int spd) : position(pos), speed(spd) {}

    void update() {
        position += speed;
    }

    void display() {
        std::cout << "Vehicle at position: " << position << " with speed: " << speed << std::endl;
    }
};

class TrafficSimulation {
    std::vector<Vehicle> vehicles;

public:
    TrafficSimulation(int numVehicles) {
        srand(time(NULL));
        for (int i = 0; i < numVehicles; i++) {
            vehicles.emplace_back(rand() % 100, rand() % 5 + 1);
        }
    }

    void step() {
        for (auto &vehicle : vehicles) {
            vehicle.update();
        }
    }

    void display() {
        for (auto &vehicle : vehicles) {
            vehicle.display();
        }
        std::cout << "---------------------------" << std::endl;
    }
};

int main() {
    TrafficSimulation simulation(5);

    for (int i = 0; i < 10; i++) {
        simulation.display();
        simulation.step();
    }

    return 0;
}
