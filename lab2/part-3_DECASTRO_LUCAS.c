#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Ajout de la library pour la fonction malloc

void affichetableau(char **sentence, int nbmot, int max) {
    int i, j;

    for (i = 0; i < nbmot; i++) {
        printf("%s\n", sentence[i]);
    }
}

int main(void) {
    char **words;
    char sentence[150];
    int i = 0, j = 0, k = 0, max = 0, word = 1; // Initialisez word à 1 pour inclure le dernier mot

    printf("Entrez votre phrase: ");
    scanf(" %[^\n]", sentence); // Format correct pour lire la phrase, y compris les espaces

    for (i = 0; i < strlen(sentence); i++) {
        if (sentence[i] == ' ') {
            word++;
            if (k > max) {
                max = k;
            }
            k = 0;
        }
        k++;
    }

    words = (char **)malloc(sizeof(char *) * word);

    for (i = 0; i < word; i++) {
        words[i] = (char *)malloc(sizeof(char) * (max + 1)); // +1 pour le caractère nul de fin de string
    }

    i = 0;
    j = 0;
    k = 0;

    while (sentence[i] != '\0') { // Correction de la parenthèse ici
        if (sentence[i] != ' ') {
            words[j][k++] = sentence[i];
        } else {
            words[j++][k] = '\0';
            k = 0;
        }
        i++; // Incrémentation de i ici
    }

    words[j][k] = '\0'; // On termine la string

    affichetableau(words, word, max);

    return 0;
}
