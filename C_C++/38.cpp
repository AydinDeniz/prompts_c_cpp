
#include <iostream>
#include <vector>
#include <cmath>

class Robot {
public:
    double position;
    double velocity;
    double acceleration;

    Robot() : position(0), velocity(0), acceleration(0) {}

    void applyForce(double force, double mass) {
        acceleration = force / mass;
    }

    void update(double timeStep) {
        velocity += acceleration * timeStep;
        position += velocity * timeStep;
    }

    void displayState() {
        std::cout << "Position: " << position << " | Velocity: " << velocity << " | Acceleration: " << acceleration << std::endl;
    }
};

int main() {
    Robot robot;

    double timeStep = 0.1;
    for (int i = 0; i < 20; i++) {
        robot.applyForce(10.0, 5.0);
        robot.update(timeStep);
        robot.displayState();
    }

    return 0;
}
