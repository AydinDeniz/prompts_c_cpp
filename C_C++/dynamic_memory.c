#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = NULL;
    int size = 0;
    int capacity = 2;  // Initial capacity
    int value;

    // Allocate initial memory for the array
    arr = (int*)malloc(capacity * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter integers (type -1 to stop):\n");

    while (1) {
        printf("Enter a value: ");
        scanf("%d", &value);

        if (value == -1) {
            break;
        }

        // Resize array if needed
        if (size == capacity) {
            capacity *= 2;
            int *temp = (int*)realloc(arr, capacity * sizeof(int));
            if (temp == NULL) {
                printf("Memory reallocation failed.\n");
                free(arr);  // Free previously allocated memory
                return 1;
            }
            arr = temp;
        }

        arr[size++] = value;  // Add the new element and increment size
    }

    // Print the elements in the array
    printf("You entered:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Free allocated memory
    free(arr);

    return 0;
}