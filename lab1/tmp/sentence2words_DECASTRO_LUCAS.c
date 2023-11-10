#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeNewline(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int countWords(const char* sentence) {
    int wordCount = 1;
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (sentence[i] == ' ') {
            wordCount++;
        }
    }
    return wordCount;
}

void splitSentence(const char* sentence, char** words, int wordCount) {
    char* word = strtok((char*)sentence, " ");
    for (int i = 0; i < wordCount; i++) {
        words[i] = word;
        word = strtok(NULL, " ");
    }
}

void printWords(char** words, int wordCount) {
    for (int i = 0; i < wordCount; i++) {
        printf("%s\n", words[i]);
    }
}

int main(void) {
    char inputSentence[100];
    printf("Enter a sentence: ");
    fgets(inputSentence, sizeof(inputSentence), stdin);

    removeNewline(inputSentence);

    int wordCount = countWords(inputSentence);

    char** words = (char**)malloc(wordCount * sizeof(char*));
    if (words == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }


    splitSentence(inputSentence, words, wordCount);

    printWords(words, wordCount);

    free(words);

    return 0;
}
