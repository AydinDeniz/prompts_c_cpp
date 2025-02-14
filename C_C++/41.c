
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMORY_SIZE 1024
#define MAX_PROCESSES 5

typedef struct {
    int pid;
    int priority;
    int execution_time;
} Process;

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    int free_memory;
} MemoryManager;

Process process_table[MAX_PROCESSES];
MemoryManager mem_manager = { .free_memory = MEMORY_SIZE };

void initialize_processes() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = i + 1;
        process_table[i].priority = rand() % 10;
        process_table[i].execution_time = rand() % 100 + 10;
    }
}

void allocate_memory(int pid, int size) {
    if (mem_manager.free_memory >= size) {
        mem_manager.free_memory -= size;
        printf("Process %d allocated %d bytes. Free memory: %d bytes\n", pid, size, mem_manager.free_memory);
    } else {
        printf("Memory allocation failed for process %d\n", pid);
    }
}

void deallocate_memory(int pid, int size) {
    mem_manager.free_memory += size;
    printf("Process %d deallocated %d bytes. Free memory: %d bytes\n", pid, size, mem_manager.free_memory);
}

void schedule_processes() {
    printf("Scheduling processes based on priority...\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("Executing Process %d | Priority: %d | Execution Time: %d\n",
               process_table[i].pid, process_table[i].priority, process_table[i].execution_time);
    }
}

void system_call_handler(int syscall, int param) {
    switch (syscall) {
        case 1:
            allocate_memory(param, 50);
            break;
        case 2:
            deallocate_memory(param, 50);
            break;
        default:
            printf("Invalid system call!\n");
    }
}

int main() {
    printf("Initializing Kernel...\n");
    initialize_processes();

    system_call_handler(1, 1);
    schedule_processes();
    system_call_handler(2, 1);

    return 0;
}
