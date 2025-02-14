
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#define MAX_FAULTS 5
#define BUFFER_SIZE 256

typedef struct {
    int fault_id;
    char description[BUFFER_SIZE];
    void (*inject_fault)();
} Fault;

void memory_corruption() {
    printf("[FAULT] Injecting memory corruption...\n");
    char *ptr = (char*)malloc(10);
    strcpy(ptr, "This is a long string that exceeds allocated memory...");
    printf("[FAULT] Corrupted memory: %s\n", ptr);
}

void stack_overflow() {
    printf("[FAULT] Simulating stack overflow...\n");
    stack_overflow();
}

void null_pointer_dereference() {
    printf("[FAULT] Injecting null pointer dereference...\n");
    char *ptr = NULL;
    *ptr = 'X';
}

void divide_by_zero() {
    printf("[FAULT] Injecting divide by zero error...\n");
    int x = 10 / 0;
}

void infinite_loop() {
    printf("[FAULT] Injecting infinite loop...\n");
    while (1);
}

Fault fault_list[MAX_FAULTS] = {
    {1, "Memory Corruption", memory_corruption},
    {2, "Stack Overflow", stack_overflow},
    {3, "Null Pointer Dereference", null_pointer_dereference},
    {4, "Divide by Zero", divide_by_zero},
    {5, "Infinite Loop", infinite_loop}
};

void *inject_random_fault(void *arg) {
    srand(time(NULL));
    int fault_index = rand() % MAX_FAULTS;
    printf("[INFO] Selected fault: %s\n", fault_list[fault_index].description);
    fault_list[fault_index].inject_fault();
    return NULL;
}

void signal_handler(int signum) {
    printf("[ERROR] Process crashed due to fault injection!\n");
    exit(1);
}

int main() {
    signal(SIGSEGV, signal_handler);
    signal(SIGFPE, signal_handler);

    pthread_t fault_thread;
    printf("[INFO] Starting Fault Injection Framework...\n");
    
    pthread_create(&fault_thread, NULL, inject_random_fault, NULL);
    pthread_join(fault_thread, NULL);

    return 0;
}
