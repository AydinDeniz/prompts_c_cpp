
#include <stdio.h>
#include <stdlib.h>

// Define the block structure with a size and a free/allocated status
typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

#define BLOCK_SIZE sizeof(Block)

// Head of the linked list
Block* freeList = NULL;

// Helper function to find a free block with enough space (first-fit)
Block* findFreeBlock(Block** last, size_t size) {
    Block* current = freeList;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

// Helper function to request a block from the OS
Block* requestSpace(Block* last, size_t size) {
    Block* block = (Block*)sbrk(size + BLOCK_SIZE);
    if (block == (void*)-1) return NULL; // sbrk failed
    block->size = size;
    block->free = 0;
    block->next = NULL;
    if (last) last->next = block;
    return block;
}

// Custom malloc function
void* myMalloc(size_t size) {
    if (size <= 0) return NULL;
    Block* block;
    if (!freeList) { // First call
        block = requestSpace(NULL, size);
        if (!block) return NULL; // Failed to allocate
        freeList = block;
    } else {
        Block* last = freeList;
        block = findFreeBlock(&last, size);
        if (!block) { // No free block found
            block = requestSpace(last, size);
            if (!block) return NULL; // Failed to allocate
        } else {
            block->free = 0; // Mark as allocated
        }
    }
    return (block + 1); // Return pointer to the data part of the block
}

// Helper function to merge free blocks (for coalescing)
void mergeFreeBlocks() {
    Block* current = freeList;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

// Custom free function
void myFree(void* ptr) {
    if (!ptr) return;
    Block* block = (Block*)ptr - 1; // Get the block metadata
    block->free = 1; // Mark as free
    mergeFreeBlocks(); // Merge adjacent free blocks
}

// Print the list of blocks for debugging
void printMemoryBlocks() {
    Block* current = freeList;
    printf("Memory Blocks:\n");
    while (current) {
        printf("Block at %p, Size: %zu, Free: %d\n", (void*)current, current->size, current->free);
        current = current->next;
    }
    printf("\n");
}

// Main function to test
int main() {
    int* p1 = (int*)myMalloc(sizeof(int) * 10); // Allocate space for 10 integers
    printMemoryBlocks();
    
    int* p2 = (int*)myMalloc(sizeof(int) * 5);  // Allocate space for 5 integers
    printMemoryBlocks();
    
    myFree(p1);  // Free the first block
    printMemoryBlocks();
    
    int* p3 = (int*)myMalloc(sizeof(int) * 2);  // Allocate space for 2 integers
    printMemoryBlocks();
    
    myFree(p2);  // Free the second block
    printMemoryBlocks();
    
    myFree(p3);  // Free the third block
    printMemoryBlocks();
    
    return 0;
}
