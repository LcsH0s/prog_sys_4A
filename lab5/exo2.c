#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIZE_FIFO 8
#define NB_PROD 1
#define NB_CONSO 1
#define NB_MESSAGE 10 // Number of messages a producer sends
#define DEBUG_MODE 1
pthread_mutex_t fifoMutex;
pthread_cond_t fifoNotEmptyCond;
pthread_cond_t fifoNotFullCond;

// Structure Fifo communication entre consommateurs et producteurs
typedef struct fifo {
  int tab[SIZE_FIFO]; // Tableau d'éléments stockés
  int ptr_lecteur;    // Index de la prochaine case à lire
  int ptr_ecrivain;   // Index de la prochaine case à écrire
  int nb_elem;        // Nombre d'éléments présents dans la fifo
} fifo;

// Fonction d'initialisation de la fifo
void init_fifo(struct fifo *f) {
  (*f).ptr_ecrivain = 0;
  (*f).ptr_lecteur = -1; // -1 meaning there is nothing to read
  (*f).nb_elem = 0;
}

// Fonction permettant d'insérer une valeur dans la fifo
void put(struct fifo *f, int a) {
  if ((*f).ptr_ecrivain == 7){
    printf("[ERROR] fut function : cannot add element to saturated queue");
    return;
  }
  (*f).tab[(*f).ptr_ecrivain] = a;
  
}

// Fonction permettant de récupérer une valeur depuis la fifo
int get(struct fifo *f) {}

// Fonction de production
void *prod(void *arg) {}

// Fonction de consommation
void *conso(void *arg) {}

// Fonction principale
int main() {
  pthread_t pidProd[NB_PROD];   // pid array of producers
  pthread_t pidConso[NB_CONSO]; // pid array of consumers
  fifo f;                       // Fifo struct

  pthread_mutex_init(&fifoMutex, NULL);
  pthread_cond_init(&fifoNotEmptyCond, NULL);
  pthread_cond_init(&fifoNotFullCond, NULL);
  init_fifo(&f); // Fifo struct initialization

  srand((unsigned int)time(NULL)); // Seed rand on time

  for (int i = 0; i < NB_PROD; i++) {
    pthread_create(&pidProd[i], NULL, prod, (void *)&f);
  }

  for (int i = 0; i < NB_CONSO; i++) {
    pthread_create(&pidConso[i], NULL, conso, (void *)&f);
  }

  for (int i = 0; i < NB_PROD; ++i) {
    pthread_join(pidProd[i], NULL);
  }

  for (int i = 0; i < NB_CONSO; ++i) {
    pthread_join(pidConso[i], NULL);
  }

  printf("All threads have completed their tasks\n");
}
