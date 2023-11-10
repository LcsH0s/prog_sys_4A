#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NB_TRAVAILLEUR 4

int travail;
pthread_mutex_t mutex;

void* travailleur(void* arg) {
    printf("Bonjour travailleur \n");
    for (;;) {
        if (pthread_mutex_trylock(&mutex)==0) {
            if (travail > 0) {
                travail -= 10;
                printf("Travail consomme : %d\n", travail);
            }
            else {
                printf("POLLING\n");
            }
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

void* patron(void* arg) {
    printf("Bonjour patron \n");
    for (int i = 0;i < 3;i ++) {
        if (pthread_mutex_trylock(&mutex)==0) {
        travail += 100;
            pthread_mutex_unlock(&mutex);

        printf("Travail produit par le patron : %d\n", travail);
        }
        sleep(6);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t patron_id;
    pthread_t travailleur_id[NB_TRAVAILLEUR];
    
    travail = 0;
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&patron_id, NULL, patron, NULL);
    for (int i = 0; i < NB_TRAVAILLEUR; i++) {
        pthread_create(&travailleur_id[i], NULL, travailleur, NULL);
    }
    for(;;);
}
