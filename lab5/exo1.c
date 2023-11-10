#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
long global = 0;
pthread_mutex_t monMutex = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *arg) {
  pthread_mutex_lock(&monMutex);
  long i = (long)arg;
  printf("Bonjour, j'ai reçu %ld\n", i);

  global += i;
  pthread_mutex_unlock(&monMutex);
  int r = (long)arg * 10;
  pthread_exit((void *)(intptr_t)r);
}

int main() {
  long i;

  pthread_t pid[N];
  for (i = 0; i < N; i++) {
    pthread_create(&pid[i], NULL, thread, (void *)i);
  }

  long results[N];

  for (int i = 0; i < N; ++i) {
    void *result;
    pthread_join(pid[i], &result);
    results[i] = (long)(intptr_t)result;
  }

  printf("\nValeurs retournées par les tâches :\n");
  for (int i = 0; i < N; ++i) {
    printf("%ld ", results[i]);
  }
  printf("\n");

  printf("\nLa valeur globale est maintenant de valeur : %ld\n", global);
  long expected_value = ((N - 1) * ((N - 1) + 1)) / 2;
  printf("Valeur attendue : %ld\n", expected_value);

  printf("Fin du thread principal\n");
  return EXIT_SUCCESS;
}