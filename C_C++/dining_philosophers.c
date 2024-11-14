
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5 // Number of philosophers

pthread_mutex_t forks[N]; // Mutexes representing forks
pthread_t philosophers[N]; // Threads for philosophers

// Function for each philosopher's behavior
void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        usleep(rand() % 1000); // Thinking

        // To avoid deadlock, ensure the philosopher picks up the lower-numbered fork first
        int leftFork = id;
        int rightFork = (id + 1) % N;

        if (id % 2 == 0) { // Even philosopher
            pthread_mutex_lock(&forks[leftFork]);
            pthread_mutex_lock(&forks[rightFork]);
        } else { // Odd philosopher
            pthread_mutex_lock(&forks[rightFork]);
            pthread_mutex_lock(&forks[leftFork]);
        }

        // Eating
        printf("Philosopher %d is eating.\n", id);
        usleep(rand() % 1000); // Eating

        // Put down forks
        pthread_mutex_unlock(&forks[leftFork]);
        pthread_mutex_unlock(&forks[rightFork]);
        
        printf("Philosopher %d finished eating and is thinking again.\n", id);
    }

    return NULL;
}

int main() {
    int ids[N];
    
    // Initialize the mutexes for each fork
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to finish (they won't in this infinite loop)
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes (unreachable in this implementation due to infinite loop)
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
