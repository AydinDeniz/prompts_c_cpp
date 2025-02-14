#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash table states for quadratic probing
#define EMPTY 0
#define OCCUPIED 1
#define DELETED 2

typedef struct {
    int key;
    int value;
    int state;
} HashEntry;

typedef struct {
    HashEntry* table;
    int capacity;
    int size;
    int collisions;
} HashTable;

unsigned int hashFunction(int key, int capacity) {
    return key % capacity;
}

unsigned int doubleHash(int key, int capacity) {
    return 1 + (key % (capacity - 2));
}

HashTable* createHashTable(int capacity) {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->capacity = capacity;
    hashTable->size = 0;
    hashTable->collisions = 0;
    hashTable->table = (HashEntry*)malloc(capacity * sizeof(HashEntry));
    
    for (int i = 0; i < capacity; i++) {
        hashTable->table[i].state = EMPTY;
    }
    
    return hashTable;
}

void resizeHashTable(HashTable* hashTable) {
    int oldCapacity = hashTable->capacity;
    int newCapacity = oldCapacity * 2;
    HashEntry* newTable = (HashEntry*)malloc(newCapacity * sizeof(HashEntry));
    
    for (int i = 0; i < newCapacity; i++) {
        newTable[i].state = EMPTY;
    }

    for (int i = 0; i < oldCapacity; i++) {
        if (hashTable->table[i].state == OCCUPIED) {
            int key = hashTable->table[i].key;
            int hash = hashFunction(key, newCapacity);
            int doubleHashValue = doubleHash(key, newCapacity);

            while (newTable[hash].state == OCCUPIED) {
                hash = (hash + doubleHashValue) % newCapacity;
            }

            newTable[hash] = hashTable->table[i];
        }
    }

    free(hashTable->table);
    hashTable->table = newTable;
    hashTable->capacity = newCapacity;
}

void insert(HashTable* hashTable, int key, int value) {
    if ((double)hashTable->size / (double)hashTable->capacity >= 0.7) {
        resizeHashTable(hashTable);
    }

    int hash = hashFunction(key, hashTable->capacity);
    int doubleHashValue = doubleHash(key, hashTable->capacity);
    int probeCount = 0;

    while (hashTable->table[hash].state == OCCUPIED) {
        if (hashTable->table[hash].key == key) {
            hashTable->table[hash].value = value;
            return;
        }
        hash = (hash + doubleHashValue) % hashTable->capacity;
        probeCount++;
    }

    hashTable->table[hash].key = key;
    hashTable->table[hash].value = value;
    hashTable->table[hash].state = OCCUPIED;
    hashTable->size++;
    hashTable->collisions += probeCount > 0 ? probeCount : 0;
}

int search(HashTable* hashTable, int key) {
    int hash = hashFunction(key, hashTable->capacity);
    int doubleHashValue = doubleHash(key, hashTable->capacity);
    int counter = 0;

    while (hashTable->table[hash].state != EMPTY && counter < hashTable->capacity) {
        if (hashTable->table[hash].state == OCCUPIED && hashTable->table[hash].key == key) {
            return hashTable->table[hash].value;
        }
        hash = (hash + doubleHashValue) % hashTable->capacity;
        counter++;
    }
    
    return -1;
}

void removeItem(HashTable* hashTable, int key) {
    int hash = hashFunction(key, hashTable->capacity);
    int doubleHashValue = doubleHash(key, hashTable->capacity);
    int counter = 0;

    while (hashTable->table[hash].state != EMPTY && counter < hashTable->capacity) {
        if (hashTable->table[hash].state == OCCUPIED && hashTable->table[hash].key == key) {
            hashTable->table[hash].state = DELETED;
            hashTable->size--;
            return;
        }
        hash = (hash + doubleHashValue) % hashTable->capacity;
        counter++;
    }
}

void displayDiagnostics(HashTable* hashTable) {
    printf("Hash Table Capacity: %d\n", hashTable->capacity);
    printf("Hash Table Size: %d\n", hashTable->size);
    printf("Load Factor: %.2f\n", (double)hashTable->size / (double)hashTable->capacity);
    printf("Collisions: %d\n", hashTable->collisions);
}

void freeHashTable(HashTable* hashTable) {
    free(hashTable->table);
    free(hashTable);
}

int main() {
    HashTable* hashTable = createHashTable(10);

    insert(hashTable, 1, 20);
    insert(hashTable, 2, 70);
    insert(hashTable, 12, 80);
    insert(hashTable, 22, 25);
    insert(hashTable, 32, 44);
    insert(hashTable, 42, 32);
    insert(hashTable, 52, 11);
    insert(hashTable, 62, 78);
    insert(hashTable, 72, 97);

    printf("Value for key 32 is: %d\n", search(hashTable, 32));
    printf("Value for key 99 is: %d\n", search(hashTable, 99));

    removeItem(hashTable, 32);
    printf("Value for key 32 after removal is: %d\n", search(hashTable, 32));

    displayDiagnostics(hashTable);

    freeHashTable(hashTable);
    
    return 0;
}