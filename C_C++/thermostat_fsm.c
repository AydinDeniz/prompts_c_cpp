
#include <stdio.h>

typedef enum {
    OFF,
    HEATING,
    COOLING
} State;

typedef struct {
    State currentState;
    double temperature;
    double targetTemperature;
} ThermostatFSM;

// Function to transition the FSM based on the current state and temperature
void transition(ThermostatFSM* fsm) {
    switch (fsm->currentState) {
        case OFF:
            if (fsm->temperature < fsm->targetTemperature - 1.0) {
                fsm->currentState = HEATING;
                printf("Transition to HEATING state.\n");
            } else if (fsm->temperature > fsm->targetTemperature + 1.0) {
                fsm->currentState = COOLING;
                printf("Transition to COOLING state.\n");
            }
            break;
        case HEATING:
            if (fsm->temperature >= fsm->targetTemperature) {
                fsm->currentState = OFF;
                printf("Transition to OFF state.\n");
            }
            break;
        case COOLING:
            if (fsm->temperature <= fsm->targetTemperature) {
                fsm->currentState = OFF;
                printf("Transition to OFF state.\n");
            }
            break;
    }
}

// Function to simulate temperature change and FSM control
void updateTemperature(ThermostatFSM* fsm, double change) {
    fsm->temperature += change;
    printf("Current temperature: %.2f\n", fsm->temperature);
    transition(fsm);
}

int main() {
    ThermostatFSM fsm = {OFF, 20.0, 22.0}; // Initial state, current temperature, target temperature

    // Simulate temperature changes
    double temperatureChanges[] = {1.0, 1.0, -0.5, -1.0, -1.0, 1.5, 2.0, -3.0};
    int n = sizeof(temperatureChanges) / sizeof(temperatureChanges[0]);

    for (int i = 0; i < n; i++) {
        updateTemperature(&fsm, temperatureChanges[i]);
    }

    return 0;
}
