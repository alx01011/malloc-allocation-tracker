#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc_list.h"

#define NUM_THREADS 4

void *malloc_and_free(void *nullptr) {
  (void)nullptr;

  for (int i = 0; i < 50; i++) {
    void *ptr = malloc(100);
    free(ptr);
  }
  return 0;
}

void *malloc_and_leak(void *nullptr) {
  (void)nullptr;

  for (int i = 0; i < 50; i++) {
    void *ptr = malloc(10);
  }

  return NULL;
}

int main(void) {
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, malloc_and_free, NULL);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  fprintf(stderr, "Active allocations after freeing everything:\n");
  print_active_allocations(allocs);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, malloc_and_leak, NULL);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  fprintf(stderr, "\nActive allocations after leaking everything:\n");

  print_active_allocations(allocs);

}
