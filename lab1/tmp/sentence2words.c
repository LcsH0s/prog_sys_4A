#include <stdio.h>

int main(void) {
    char sentence[] = "a little step for the man";
    char words[6][10];
    int i = 0, j = 0, k = 0;

    do {
        if (sentence[i] != ' ')
            words[j][k++] = sentence[i];
        else {
            words[j++][k] = '\0';
            k = 0;
        }
    } while (sentence[i++] != '\0');

    for (i = 0; i < 6; i++)
        printf("%s\n", words[i]);

    return 0;
}
