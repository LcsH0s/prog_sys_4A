#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIZE_FIFO 8
#define NB_PROD 2
#define NB_CONSO 2
#define NB_MESSAGE 10 // Number of messages a producer sends
#define DEBUG_MODE 1

// Structure Fifo communication entre consommateurs et producteurs
typedef struct fifo {
  int tab[SIZE_FIFO]; // Tableau d'éléments stockés
  int ptr_lecteur;    // Index de la prochaine case à lire
  int ptr_ecrivain;   // Index de la prochaine case à écrire
  int nb_elem;        // Nombre d'éléments présents dans la fifo

  pthread_mutex_t fifoMutex;
  pthread_cond_t fifoNotEmptyCond;
  pthread_cond_t fifoNotFullCond;
} fifo;

// Fonction d'initialisation de la fifo
void init_fifo(fifo *f) {
  f->ptr_ecrivain = 0;
  f->ptr_lecteur = 0; // -1 meaning there is nothing to read
  f->nb_elem = 0;

  pthread_mutex_init(&f->fifoMutex, NULL);
  pthread_cond_init(&f->fifoNotEmptyCond, NULL);
  pthread_cond_init(&f->fifoNotFullCond, NULL);
}

// Fonction permettant d'insérer une valeur dans la fifo
void put(struct fifo *f, int a) {
  pthread_mutex_lock(&f->fifoMutex);
  if (f->nb_elem == SIZE_FIFO) {
    pthread_cond_wait(&f->fifoNotFullCond, &f->fifoMutex);
  }
  f->tab[f->ptr_ecrivain] = a;
  f->ptr_ecrivain = (f->ptr_ecrivain + 1) % SIZE_FIFO;
  f->nb_elem++;

  pthread_cond_signal(&f->fifoNotEmptyCond);
  pthread_mutex_unlock(&f->fifoMutex);
  sched_yield();
}

// Fonction permettant de récupérer une valeur depuis la fifo
int get(struct fifo *f) {
  int r;
  pthread_mutex_lock(&f->fifoMutex);
  if (f->nb_elem == 0) {
    pthread_cond_wait(&f->fifoNotEmptyCond, &f->fifoMutex);
  }
  r = f->tab[f->ptr_lecteur];
  f->ptr_lecteur = (f->ptr_lecteur + 1) % SIZE_FIFO;
  f->nb_elem--;

  pthread_cond_signal(&f->fifoNotFullCond);
  pthread_mutex_unlock(&f->fifoMutex);
  sched_yield();
  return r;
}

// Fonction de production
void *prod(void *arg) {
  fifo *f = (fifo *)arg;

  for (int i = 0; i < NB_MESSAGE; i++) {
    put(f, rand());
  }
  return NULL;
}

// Fonction de consommation
void *conso(void *arg) {
  fifo *f = (fifo *)arg;
  int msg;

  for (int i = 0; i < NB_MESSAGE; i++) {
    msg = get(f);
    printf("Received : %d\n", msg);
    fflush(stdout);
  }
  return NULL;
}

// Fonction principale
int main() {
  pthread_t pidProd[NB_PROD];   // pid array of producers
  pthread_t pidConso[NB_CONSO]; // pid array of consumers
  fifo f;                       // Fifo struct

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
