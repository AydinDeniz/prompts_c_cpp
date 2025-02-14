#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Block struct for the custom memory allocator
typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

#define BLOCK_SIZE sizeof(Block)

Block* freeList = NULL;  // Free list start

// Function to find a free block
Block* findFreeBlock(Block** last, size_t size) {
    Block* current = freeList;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

// Function to request space from the OS
Block* requestSpace(Block* last, size_t size) {
    Block* block = (Block*)sbrk(0);
    void* request = sbrk(size + BLOCK_SIZE);
    if (request == (void*) -1) 
        return NULL;  // sbrk failed

    if (last) {
        last->next = block;
    }

    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}

// Custom malloc implementation
void* customMalloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    Block* block;
    if (!freeList) {  // First caller
        block = requestSpace(NULL, size);
        if (!block) {
            return NULL;  // sbrk failed
        }
        freeList = block;
    } else {
        Block* last = freeList;
        block = findFreeBlock(&last, size);
        if (!block) {  // No fitting block found
            block = requestSpace(last, size);
            if (!block) {
                return NULL;  // sbrk failed
            }
        } else {  // Found free block
            block->free = 0;
        }
    }
    return (block + 1);
}

// Function to get the block from a memory address
Block* getBlockPtr(void* ptr) {
    return (Block*)ptr - 1;
}

// Custom free implementation
void customFree(void* ptr) {
    if (!ptr) {
        return;
    }

    Block* block = getBlockPtr(ptr);
    block->free = 1;
}

// Function to combine adjacent free blocks (defragmentation)
void defragment() {
    Block* current = freeList;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

// Function to display the memory allocation status
void printMemoryStatus() {
    Block* current = freeList;
    printf("Memory Blocks:\n");
    while (current) {
        printf("Block Size: %zu, Free: %d\n", current->size, current->free);
        current = current->next;
    }
}

int main() {
    int* ptr1 = (int*)customMalloc(sizeof(int) * 10);
    int* ptr2 = (int*)customMalloc(sizeof(int) * 20);

    printMemoryStatus();

    customFree(ptr1);
    customFree(ptr2);

    defragment();
    printMemoryStatus();

    return 0;
}