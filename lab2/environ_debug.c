#include <stdio.h>

extern char** environ;

int main() {
    int c = 0;
    
    while (environ[c] != NULL) {
        c++;
    }
    
    printf("Nombre de variables système : %d\n", c);
    
    return 0;
}
