#include <assert.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TAB_SIZE 100000

struct context {
  int *ptrStart;
  int *ptrEnd;
  int startIndex;
  int threadCount;
};

pthread_t *tid;

double getTime() {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return now.tv_sec + now.tv_nsec * 1e-9;
}

int *initTab(int s) {
  srand(time(NULL));
  int *t = malloc(sizeof(int) * s);

  for (int i = 0; i < s; i++) {
    t[i] = i;
  }

  for (int i = 0; i < s; i++) {
    t[i] = i;
  }

  for (int i = s - 1; i > 0; i--) {
    int j = rand() % (i + 1);

    int temp = t[i];
    t[i] = t[j];
    t[j] = temp;
  }

  return t;
}

void displayTab(int *t, int s) {
  if (s <= 100) {
    printf("Tableau : ");
    for (int i = 0; i < s; i++) {
      printf("%d ", t[i]);
    }
    printf("\n");
  } else {
    printf("Tableau non affiché car sa taille dépasse 100 éléments.\n");
  }
}

char isOrdered(int *t, int s) {
  int prev = t[0];
  for (int i = 1; i < s; i++) {
    if (prev > t[i]) {
      printf("Not ordered on index %d. %d should have been %d\n", i, t[i], i);
      return 0;
    }
    prev = t[i];
  }
  return 1;
}

int cmp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void partialSort(int *s, int *e) { qsort(s, e - s, sizeof(int), cmp); }

void mergeTab(int *s1, const int *e1, int *s2, const int *e2) {
  int *start = s1;

  if (s2 == e2)
    return;

  assert((e1 - s1) > 0);
  assert((e2 - s2) > 0);

  int size = e2 - s1;
  int *out = malloc(sizeof(int) * size);
  int *tmp = out;

  while (!(s1 >= e1 || s2 >= e2)) {
    if (*s1 > *s2) {
      *out = *s2;
      ++s2;
    } else {
      *out = *s1;
      ++s1;
    }
    ++out;
  }

  while (s2 < e2) {
    *out = *s2;
    ++out;
    ++s2;
  }

  while (s1 < e1) {
    *out = *s1;
    ++out;
    ++s1;
  }

  memcpy(start, tmp, size * sizeof(int));
  free(tmp);
}

void *sortThread(void *arg) {
  struct context *ctx = arg;
  size_t iterNumber = (size_t)ceil(log2((double)ctx->threadCount));

  partialSort(ctx->ptrStart, ctx->ptrEnd);

  for (size_t iteration = 0; iteration < iterNumber; ++iteration) {
    size_t currentBlockSize = 1 * (size_t)pow(2, iteration + 1);

    if ((ctx->startIndex % currentBlockSize) == 0) {
      size_t awaitedThreadIndex =
          ctx->startIndex + (1 * (size_t)pow(2, iteration));

      if (awaitedThreadIndex >= ctx->threadCount) {
        return ctx;
      }

      struct context *mergingCtx;
      assert(pthread_join(tid[awaitedThreadIndex], (void **)&mergingCtx) == 0);

      mergeTab(ctx->ptrStart, ctx->ptrEnd, mergingCtx->ptrStart,
               mergingCtx->ptrEnd);

      ctx->ptrEnd = mergingCtx->ptrEnd;

      free(mergingCtx);
    } else {
      return ctx;
    }
  }
  return ctx;
}

void mainSortingThread(int t[], int threadCount) {
  int sliceSize = ceil((double)TAB_SIZE / threadCount);
  tid = malloc(threadCount * sizeof(pthread_t));

  if (tid == NULL) {
    perror("Error allocating memory for tid");
    exit(EXIT_FAILURE);
  }

  int i;
  for (i = threadCount - 1; i >= 0; --i) {
    struct context *ctx = malloc(sizeof(struct context));
    if (ctx == NULL) {
      perror("Error allocating memory for context");
      exit(EXIT_FAILURE);
    }

    ctx->ptrStart = t + i * sliceSize;
    ctx->ptrEnd = t + (i + 1) * sliceSize;
    if (ctx->ptrEnd > t + TAB_SIZE)
      ctx->ptrEnd = t + TAB_SIZE;
    ctx->startIndex = i;
    ctx->threadCount = threadCount;
    assert(pthread_create(&tid[i], NULL, sortThread, ctx) == 0);
  }

  struct context *ctx;
  assert(pthread_join(tid[0], (void **)&ctx) == 0);
  free(ctx);
}

int main(int argc, char *argv[]) {
  int *t = initTab(TAB_SIZE);

  if (argc != 2) {
    printf("Usage: %s <nombre superieur a 0>\n", argv[0]);
    return 1;
  }

  int threadCount = atoi(argv[1]); // Convertir l'argument en entier

  if (threadCount <= 0) {
    printf("Veuillez fournir un nombre superieur a 0.\n");
    return 1;
  }

  displayTab(t, TAB_SIZE);
  double timeTaken = getTime();
  mainSortingThread(t, threadCount);
  timeTaken = getTime() - timeTaken;
  displayTab(t, TAB_SIZE);

  if (isOrdered(t, TAB_SIZE)) {
    printf("Le tableau est ordonné !\n");
  } else {
    printf("Le tableau n'est pas ordonné :(\n");
  }

  printf("Le tri a pris %f secondes a s'executer\n", timeTaken);

  FILE *fichier = fopen("data_100000.csv", "a");

  // Vérification si le fichier a pu être ouvert
  if (fichier == NULL) {
    printf("Erreur lors de l'ouverture du fichier de donnees\n");
    return 1;
  }

  // Écriture des valeurs dans le fichier CSV
  fprintf(fichier, "%d,%f\n", threadCount, timeTaken);

  // Fermeture du fichier
  fclose(fichier);

  printf("Les valeurs ont été ajoutées avec succès.\n");

  return 0;
}