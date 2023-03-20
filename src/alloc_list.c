/****************************************************************************
  MIT License

Copyright (c) 2023 Alexandros Emmanouil Antonakakis <antonakakis1@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/



/*
    *  alloc_list.c
    *  Author: Alexandros Antonakakis
    *  Created on: 18-03-2023
    *  Description: Implementation of a linked list for keeping track of
    *                allocations
*/

#include "alloc_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>

/* define a global allocs list variable */

alloc_list *allocs = NULL;

/* since we are replacing malloc with  a custom  function,
which internally calls libc malloc, using malloc again,
will cause infinite recursion.
Instead we use mmap to allocate memory for the alloc_list
*/

alloc_list *alloc_list_add(alloc_list *list, void *ptr, size_t size) {
  alloc_list *new = mmap(0, sizeof(alloc_list), PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (new == MAP_FAILED) {
    fprintf(stderr, "error: mmap %s in function %s\n)", strerror(errno), __func__);
    exit(EXIT_FAILURE);
  }

  new->ptr        = ptr;
  new->next       = list;
  new->size       = size;
  new->tid        = pthread_self();
  return new;
}

alloc_list *alloc_list_remove(alloc_list *list, void *ptr) {
  alloc_list *cur = 0, *prev = 0;

  cur = list;

  for (; cur && ptr != cur->ptr; prev = cur, cur = cur->next)
    ;

  if (!cur) {
    return list;  // not found
  }

  if (prev) {
    prev->next = cur->next;
  } else {
    list = cur->next;
  }

  if(munmap(cur, sizeof(alloc_list))) {
    fprintf(stderr, "error: munmap %s in function %s)", strerror(errno), __func__);
    exit(EXIT_FAILURE);
  }

  return list;
}

void alloc_list_destroy(alloc_list *list) {
  alloc_list *cur = 0, *next = 0;

  cur = list;

  while (cur) {
    next = cur->next;
    if(munmap(cur, sizeof(alloc_list))) {
      fprintf(stderr, "error: munmap %s in function %s)", strerror(errno), __func__);
      exit(EXIT_FAILURE);
    }
    cur = next;
  }
}

int is_alloc_list_empty(alloc_list *list) {
  return list == NULL; /* means everything was freed */
}

void print_active_allocations(alloc_list *list) { 
  while (list) {
    fprintf(stderr, "%p with size: %lu bytes, tid: ", list->ptr, list->size);
    for (size_t i = sizeof(size_t); i; --i)
        fprintf(stderr, "%02x", *(((unsigned char*) &list->tid) + i - 1));

    fputc('\n', stderr);
    list = list->next;
    }
}

