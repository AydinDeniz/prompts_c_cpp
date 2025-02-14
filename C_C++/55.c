#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Stack structure
typedef struct Stack {
    int top;
    unsigned capacity;
    int* array;
} Stack;

// Function to create a stack
Stack* createStack(unsigned capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is -1
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// Function to add an item to stack. Increases top by 1
void push(Stack* stack, int item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

// Function to remove an item from stack. Decreases top by 1
int pop(Stack* stack) {
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top--];
}

// Function to return the top from stack without removing it
int peek(Stack* stack) {
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top];
}

// Function to check if the character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

// Function to check precedence of operators
int precedence(char ch) {
    switch (ch) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
    }
    return -1;
}

// Function to convert infix expression to postfix
int infixToPostfix(char* exp, char* result) {
    int i, k;
    Stack* stack = createStack(strlen(exp));
    if (!stack) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    for (i = 0, k = 0; exp[i]; ++i) {
        if (isspace(exp[i]))
            continue;

        if (isdigit(exp[i])) {
            while (isdigit(exp[i])) {
                result[k++] = exp[i++];
            }
            result[k++] = ' ';
            i--;
        }
        else if (exp[i] == '(') {
            push(stack, exp[i]);
        }
        else if (exp[i] == ')') {
            while (!isEmpty(stack) && peek(stack) != '(') {
                result[k++] = (char)pop(stack);
                result[k++] = ' ';
            }
            if (!isEmpty(stack) && peek(stack) != '(')
                return -1;
            else
                pop(stack);
        }
        else if (isOperator(exp[i])) {
            while (!isEmpty(stack) && precedence(exp[i]) <= precedence(peek(stack))) {
                result[k++] = (char)pop(stack);
                result[k++] = ' ';
            }
            push(stack, exp[i]);
        }
    }

    while (!isEmpty(stack)) {
        result[k++] = (char)pop(stack);
        result[k++] = ' ';
    }

    result[k - 1] = '\0';  // Null terminate string
    free(stack);
    return 0;
}

// Function to evaluate postfix expression
int evaluatePostfix(char* exp) {
    Stack* stack = createStack(strlen(exp));
    int i;

    if (!stack) return -1;

    for (i = 0; exp[i]; i++) {
        if (isspace(exp[i]))
            continue;

        else if (isdigit(exp[i])) {
            int num = 0;
            while (isdigit(exp[i])) {
                num = num * 10 + (int)(exp[i] - '0');
                i++;
            }
            i--;
            push(stack, num);
        }

        else if (isOperator(exp[i])) {
            int val1 = pop(stack);
            int val2 = pop(stack);
            switch (exp[i]) {
                case '+':
                    push(stack, val2 + val1);
                    break;
                case '-':
                    push(stack, val2 - val1);
                    break;
                case '*':
                    push(stack, val2 * val1);
                    break;
                case '/':
                    push(stack, val2 / val1);
                    break;
                case '^':
                    push(stack, (int)pow(val2, val1));
                    break;
            }
        }
    }
    return pop(stack);
}

int main() {
    char infix[] = "3 + 5 * ( 2 ^ 3 - 2 ) ^ 2";  // Example infix expression
    char postfix[100];

    if (infixToPostfix(infix, postfix) == 0) {
        printf("Postfix expression: %s\n", postfix);
        printf("Evaluated result: %d\n", evaluatePostfix(postfix));
    } else {
        printf("Error in conversion\n");
    }

    return 0;
}