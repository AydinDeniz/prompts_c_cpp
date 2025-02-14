#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A data item
typedef struct DataItem {
    int data;
    int key;
    struct DataItem* next; // For chained collision handling
} DataItem;

#define SIZE 20

DataItem* hashArray[SIZE];

// Function to calculate hash code for a key
int hashCode(int key) {
    return key % SIZE;
}

// Function to search for a key in hash table
int search(int key) {
    int hashIndex = hashCode(key);
    DataItem* current = hashArray[hashIndex];

    while (current != NULL) {
        if (current->key == key)
            return current->data;
        current = current->next;
    }

    return -1; // Key not found
}

// Function to insert a key-value pair in the hash table
void insert(int key, int data) {
    DataItem* item = (DataItem*)malloc(sizeof(DataItem));
    item->data = data;
    item->key = key;
    item->next = NULL;

    int hashIndex = hashCode(key);

    // Insert at the head of the list in hashArray[hashIndex]
    if (hashArray[hashIndex] == NULL) {
        hashArray[hashIndex] = item;
    } else {
        item->next = hashArray[hashIndex];
        hashArray[hashIndex] = item;
    }
}

// Function to delete a key-value pair from the hash table
DataItem* deleteItem(int key) {
    int hashIndex = hashCode(key);
    DataItem* current = hashArray[hashIndex];
    DataItem* previous = NULL;

    while (current != NULL) {
        if (current->key == key) {
            if (previous == NULL) {
                hashArray[hashIndex] = current->next;
            } else {
                previous->next = current->next;
            }
            return current;
        }
        previous = current;
        current = current->next;
    }

    return NULL; // Key not found
}

// Function to display the contents of the hash table
void display() {
    for (int i = 0; i < SIZE; i++) {
        DataItem* current = hashArray[i];
        printf("Index %d: ", i);
        while (current != NULL) {
            printf("(%d,%d) ", current->key, current->data);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    insert(1, 20);
    insert(2, 70);
    insert(42, 80);
    insert(4, 25);
    insert(12, 44);
    insert(14, 32);
    insert(17, 11);
    insert(13, 78);
    insert(37, 97);

    display();

    int data = search(37);
    if (data != -1) {
        printf("Item found: %d\n", data);
    } else {
        printf("Item not found\n");
    }

    DataItem* item = deleteItem(37);
    if (item != NULL) {
        printf("Item deleted: %d\n", item->data);
        free(item);
    } else {
        printf("Item not found for deletion\n");
    }

    display();
    
    return 0;
}