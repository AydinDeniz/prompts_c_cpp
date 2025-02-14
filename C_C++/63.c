#include <stdio.h>
#include <stdlib.h>

#define MAX 10

#define PLAYER 1
#define COMPUTER 2

void initializeBoard(char board[MAX][MAX], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard(char board[MAX][MAX], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");
        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                printf("---");
                if (j < size - 1) printf("+");
            }
            printf("\n");
        }
    }
}

int checkWin(char board[MAX][MAX], int size) {
    for (int i = 0; i < size; i++) {
        // Check rows
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0] == 'X' ? PLAYER : COMPUTER;

        // Check columns
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i] == 'X' ? PLAYER : COMPUTER;
    }

    // Check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0] == 'X' ? PLAYER : COMPUTER;

    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2] == 'X' ? PLAYER : COMPUTER;

    return 0; // No winner yet
}

int isMovesLeft(char board[MAX][MAX], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 1;
    return 0;
}

int minimax(char board[MAX][MAX], int depth, int isMax, int size) {
    int score = checkWin(board, size);

    if (score == PLAYER)
        return -10;
    if (score == COMPUTER)
        return 10;

    if (!isMovesLeft(board, size))
        return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';  // AI's move
                    best = (best > minimax(board, depth + 1, !isMax, size)) ? best : minimax(board, depth + 1, !isMax, size);
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';  // Player's move
                    best = (best < minimax(board, depth + 1, !isMax, size)) ? best : minimax(board, depth + 1, !isMax, size);
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

void findBestMove(char board[MAX][MAX], int size, int* bestMove) {
    int bestVal = -1000;
    bestMove[0] = -1;
    bestMove[1] = -1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';  // AI's move
                int moveVal = minimax(board, 0, 0, size);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    bestMove[0] = i;
                    bestMove[1] = j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

void playTicTacToe(int size) {
    char board[MAX][MAX];
    initializeBoard(board, size);

    int player = PLAYER, bestMove[2];
    while (1) {
        printBoard(board, size);

        if (checkWin(board, size) || !isMovesLeft(board, size))
            break;

        if (player == PLAYER) {
            int row, col;
            printf("Enter row and column (1-3) for your move: ");
            scanf("%d%d", &row, &col);
            row--; col--;

            if (board[row][col] == ' ') {
                board[row][col] = 'X';
                player = COMPUTER;
            } else {
                printf("Invalid move! Try again.\n");
            }
        } else {
            findBestMove(board, size, bestMove);
            board[bestMove[0]][bestMove[1]] = 'O';
            player = PLAYER;
        }
    }

    int result = checkWin(board, size);
    printBoard(board, size);
    if (result == PLAYER) {
        printf("You win!\n");
    } else if (result == COMPUTER) {
        printf("Computer wins!\n");
    } else {
        printf("It's a draw!\n");
    }
}

int main() {
    playTicTacToe(3);
    return 0;
}