
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 5
#define ACTIONS 4
#define EPISODES 1000
#define LEARNING_RATE 0.1
#define DISCOUNT_FACTOR 0.9
#define EXPLORATION_RATE 0.2

typedef struct {
    int x, y;
} Position;

double Q_table[GRID_SIZE][GRID_SIZE][ACTIONS] = {0};

int get_action(Position state) {
    if ((double)rand() / RAND_MAX < EXPLORATION_RATE) {
        return rand() % ACTIONS;
    }
    int best_action = 0;
    double max_value = Q_table[state.x][state.y][0];
    for (int i = 1; i < ACTIONS; i++) {
        if (Q_table[state.x][state.y][i] > max_value) {
            max_value = Q_table[state.x][state.y][i];
            best_action = i;
        }
    }
    return best_action;
}

Position take_action(Position state, int action) {
    Position new_state = state;
    if (action == 0 && state.x > 0) new_state.x--; 
    if (action == 1 && state.x < GRID_SIZE - 1) new_state.x++; 
    if (action == 2 && state.y > 0) new_state.y--; 
    if (action == 3 && state.y < GRID_SIZE - 1) new_state.y++; 
    return new_state;
}

double get_reward(Position state) {
    return (state.x == GRID_SIZE - 1 && state.y == GRID_SIZE - 1) ? 10.0 : -0.1;
}

void train_agent() {
    for (int episode = 0; episode < EPISODES; episode++) {
        Position state = {0, 0};
        while (!(state.x == GRID_SIZE - 1 && state.y == GRID_SIZE - 1)) {
            int action = get_action(state);
            Position new_state = take_action(state, action);
            double reward = get_reward(new_state);

            double max_future_q = Q_table[new_state.x][new_state.y][0];
            for (int i = 1; i < ACTIONS; i++) {
                if (Q_table[new_state.x][new_state.y][i] > max_future_q) {
                    max_future_q = Q_table[new_state.x][new_state.y][i];
                }
            }

            Q_table[state.x][state.y][action] += LEARNING_RATE * (reward + DISCOUNT_FACTOR * max_future_q - Q_table[state.x][state.y][action]);
            state = new_state;
        }
    }
}

void print_policy() {
    printf("Optimal policy:\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int best_action = get_action((Position){i, j});
            printf("%c ", best_action == 0 ? '^' : best_action == 1 ? 'v' : best_action == 2 ? '<' : '>');
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    train_agent();
    print_policy();
    return 0;
}
