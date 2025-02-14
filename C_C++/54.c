#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to represent matrix dimensions
typedef struct {
    int row;
    int col;
} Dimension;

// Structure to represent data for each thread
typedef struct {
    int row;
    int col;
    int dim;
    int** A;
    int** B;
    int** C;
} ThreadData;

// Function to allocate memory for a matrix
int** allocateMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function to read matrix data from the user
void readMatrix(int** matrix, int rows, int cols, char matrixName) {
    printf("Enter elements for Matrix %c [%d x %d]:\n", matrixName, rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Function to print matrix data
void printMatrix(int** matrix, int rows, int cols, char matrixName) {
    printf("Matrix %c:\n", matrixName);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Thread function to calculate a single element of the result matrix
void* multiplyElement(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int k = 0; k < data->dim; k++) {
        sum += data->A[data->row][k] * data->B[k][data->col];
    }
    data->C[data->row][data->col] = sum;
    pthread_exit(0);
}

int main() {
    Dimension dimA, dimB;

    printf("Enter dimensions of Matrix A (rows cols): ");
    scanf("%d %d", &dimA.row, &dimA.col);
    printf("Enter dimensions of Matrix B (rows cols): ");
    scanf("%d %d", &dimB.row, &dimB.col);

    if (dimA.col != dimB.row) {
        printf("Error: Columns of A must match rows of B for multiplication.\n");
        return -1;
    }

    int** A = allocateMatrix(dimA.row, dimA.col);
    int** B = allocateMatrix(dimB.row, dimB.col);
    int** C = allocateMatrix(dimA.row, dimB.col);

    readMatrix(A, dimA.row, dimA.col, 'A');
    readMatrix(B, dimB.row, dimB.col, 'B');

    pthread_t threads[dimA.row][dimB.col];
    ThreadData threadData[dimA.row][dimB.col];

    for (int i = 0; i < dimA.row; i++) {
        for (int j = 0; j < dimB.col; j++) {
            threadData[i][j].row = i;
            threadData[i][j].col = j;
            threadData[i][j].dim = dimA.col;
            threadData[i][j].A = A;
            threadData[i][j].B = B;
            threadData[i][j].C = C;
            pthread_create(&threads[i][j], NULL, multiplyElement, (void*)&threadData[i][j]);
        }
    }

    for (int i = 0; i < dimA.row; i++) {
        for (int j = 0; j < dimB.col; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    printMatrix(C, dimA.row, dimB.col, 'C');

    for (int i = 0; i < dimA.row; i++) free(A[i]);
    for (int i = 0; i < dimB.row; i++) free(B[i]);
    for (int i = 0; i < dimA.row; i++) free(C[i]);
    free(A);
    free(B);
    free(C);

    return 0;
}