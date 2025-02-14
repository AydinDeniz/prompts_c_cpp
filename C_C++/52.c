#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to calculate word frequency
void calculateWordFrequency(FILE *file) {
    char word[256];
    int count = 0;
    while (fscanf(file, "%255s", word) != EOF) {
        count++;
    }
    printf("Total words: %d\n", count);
}

// Function to calculate sentence count
void calculateSentenceCount(FILE *file) {
    char ch;
    int sentences = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '.' || ch == '!' || ch == '?') {
            sentences++;
        }
    }
    printf("Total sentences: %d\n", sentences);
}

// Function to calculate average word length
void calculateAverageWordLength(FILE *file) {
    char word[256];
    int wordCount = 0, charCount = 0;
    while (fscanf(file, "%255s", word) != EOF) {
        wordCount++;
        charCount += strlen(word);
    }
    printf("Average word length: %.2f\n", (double)charCount / wordCount);
}

// Function to find the longest sentence
void findLongestSentence(FILE *file) {
    char sentence[1024], longestSentence[1024] = "";
    char ch;
    int index = 0;
    while ((ch = fgetc(file)) != EOF) {
        sentence[index++] = ch;
        if (ch == '.' || ch == '!' || ch == '?') {
            sentence[index] = '\0';
            if (strlen(sentence) > strlen(longestSentence)) {
                strcpy(longestSentence, sentence);
            }
            index = 0;
        }
    }
    printf("Longest sentence: %s\n", longestSentence);
}

// Function to convert text to base64
void convertToBase64(FILE *inputFile, FILE *outputFile) {
    char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned char input[3], output[4];
    int inputLength, i;

    while (!feof(inputFile)) {
        inputLength = 0;
        for (i = 0; i < 3; i++) {
            input[i] = fgetc(inputFile);
            if (!feof(inputFile)) {
                inputLength++;
            } else {
                input[i] = 0;
            }
        }

        if (inputLength) {
            output[0] = base64_table[input[0] >> 2];
            output[1] = base64_table[((input[0] & 0x03) << 4) | (input[1] >> 4)];
            output[2] = (inputLength > 1 ? base64_table[((input[1] & 0x0f) << 2) | (input[2] >> 6)] : '=');
            output[3] = (inputLength > 2 ? base64_table[input[2] & 0x3f] : '=');
            
            for (i = 0; i < 4; i++) {
                fputc(output[i], outputFile);
            }
        }
    }
    fprintf(outputFile, "\n");
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        printf("Could not open file.\n");
        return 1;
    }

    // Analyze text file content
    calculateWordFrequency(file);
    fseek(file, 0, SEEK_SET);

    calculateSentenceCount(file);
    fseek(file, 0, SEEK_SET);

    calculateAverageWordLength(file);
    fseek(file, 0, SEEK_SET);

    findLongestSentence(file);
    fseek(file, 0, SEEK_SET);

    // Convert text to base64 and save to new file
    FILE *outputFile = fopen("output_base64.txt", "w");
    if (!outputFile) {
        printf("Could not open output file.\n");
        fclose(file);
        return 1;
    }

    convertToBase64(file, outputFile);

    fclose(file);
    fclose(outputFile);
    return 0;
}