
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256

typedef struct {
    char line[MAX_LINE_LENGTH];
    int lineNumber;
} CodeLine;

void optimizeLoopUnrolling(CodeLine lines[], int count) {
    for (int i = 0; i < count; i++) {
        if (strstr(lines[i].line, "for") && strstr(lines[i].line, "++")) {
            printf("Optimization Suggestion at line %d: Consider loop unrolling to improve performance.\n", lines[i].lineNumber);
        }
    }
}

void optimizeInlineFunctions(CodeLine lines[], int count) {
    for (int i = 0; i < count; i++) {
        if (strstr(lines[i].line, "void") && strstr(lines[i].line, "(") && strstr(lines[i].line, ")")) {
            printf("Optimization Suggestion at line %d: Consider inlining this function for performance boost.\n", lines[i].lineNumber);
        }
    }
}

void optimizeConstantPropagation(CodeLine lines[], int count) {
    for (int i = 0; i < count; i++) {
        if (strstr(lines[i].line, "const") && strstr(lines[i].line, "=")) {
            printf("Optimization Suggestion at line %d: Consider propagating this constant to avoid redundant calculations.\n", lines[i].lineNumber);
        }
    }
}

void analyzeCode(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    CodeLine lines[MAX_LINES];
    int count = 0;

    while (fgets(lines[count].line, MAX_LINE_LENGTH, file) && count < MAX_LINES) {
        lines[count].lineNumber = count + 1;
        count++;
    }
    fclose(file);

    optimizeLoopUnrolling(lines, count);
    optimizeInlineFunctions(lines, count);
    optimizeConstantPropagation(lines, count);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_code.c>\n", argv[0]);
        return 1;
    }

    analyzeCode(argv[1]);
    return 0;
}
