
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 512

typedef struct {
    char line[MAX_LINE_LENGTH];
    int lineNumber;
} CodeLine;

void detectBufferOverflow(CodeLine lines[], int count) {
    for (int i = 0; i < count; i++) {
        if (strstr(lines[i].line, "gets(") || strstr(lines[i].line, "strcpy(")) {
            printf("Potential Buffer Overflow at line %d: %s\n", lines[i].lineNumber, lines[i].line);
        }
    }
}

void detectMemoryLeak(CodeLine lines[], int count) {
    int mallocCount = 0, freeCount = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(lines[i].line, "malloc(")) mallocCount++;
        if (strstr(lines[i].line, "free(")) freeCount++;
    }
    if (mallocCount > freeCount) {
        printf("Potential Memory Leak: %d malloc() calls but only %d free() calls.\n", mallocCount, freeCount);
    }
}

void detectRaceCondition(CodeLine lines[], int count) {
    bool threadUsed = false;
    for (int i = 0; i < count; i++) {
        if (strstr(lines[i].line, "pthread_create(")) threadUsed = true;
        if (threadUsed && strstr(lines[i].line, "globalVar")) {
            printf("Potential Race Condition at line %d: %s\n", lines[i].lineNumber, lines[i].line);
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

    detectBufferOverflow(lines, count);
    detectMemoryLeak(lines, count);
    detectRaceCondition(lines, count);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_code.c>\n", argv[0]);
        return 1;
    }

    analyzeCode(argv[1]);
    return 0;
}
