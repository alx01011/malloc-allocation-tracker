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
 * malloc_wrapper.c
 * Author: Alexandros Antonakakis
 * Created on: 18-03-2023
 * Description: Implementation of a wrapper for malloc and friends functions,
 *              which keep track of allocations on a list
 */

#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "alloc_list.h"

/*
 *  We need to keep track of the original libc malloc, calloc and free
 *  functions, so we can call them from our custom functions.
 *  We use dlsym to get the address of the original functions.
 */


/*
 * dlsym takes a handle to a library and a symbol name and returns
 * the address of the symbol in the library.
 * RTLD_NEXT is a special handle which tells dlsym to search for the
 * symbol in the next library in the search order.
 */

/* a global alloc_list variable */

extern alloc_list *allocs;

void *malloc(size_t s) {
  static  void *(*libc_malloc)(size_t) = 0;
  void *p = 0;

  if (!libc_malloc) {
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
    if (!libc_malloc) {
      const char *error = dlerror();
      write(STDERR_FILENO, error, sizeof(error));
      exit(EXIT_FAILURE);
    }
  }

  p = libc_malloc(s);

  if (p) allocs = alloc_list_add(allocs, p, s);

  return p;
}

void *calloc(size_t n, size_t s) {

  static void *(*libc_calloc)(size_t, size_t) = 0;

  void *p = 0;

  if (!libc_calloc) {
    libc_calloc = dlsym(RTLD_NEXT, "calloc");
    if (!libc_calloc) {
      const char *error = dlerror();
      write(STDERR_FILENO, error, sizeof(error));
      exit(EXIT_FAILURE);
    }
  }

  p = libc_calloc(n, s);

  if (p) allocs = alloc_list_add(allocs, p, n * s);

  return p;
}

/*
    realloc is a bit more complicated, since it can be used to both
    shrink and expand an allocation, so we need to remove the old allocation
    from the list and add the new one.
*/

void *realloc(void *p, size_t s) {
  static void *(*libc_realloc)(void *, size_t) = 0;

  void *new_p = 0;

  if (!libc_realloc) {
    libc_realloc = dlsym(RTLD_NEXT, "realloc");
    if (!libc_realloc) {
      const char *error = dlerror();
      write(STDERR_FILENO, error, sizeof(error));
      exit(EXIT_FAILURE);
    }
  }

  new_p = libc_realloc(p, s);

  if (new_p) {
    allocs = alloc_list_remove(allocs, p);
    allocs = alloc_list_add(allocs, new_p, s);
  }

  return new_p;
}

void free(void *p) {
    static void (*libc_free)(void *) = 0;

  if (!libc_free) {
    libc_free = dlsym(RTLD_NEXT, "free");
    if (!libc_free) {
      const char *error = dlerror();
      write(STDERR_FILENO, error, sizeof(error));
      exit(EXIT_FAILURE);
    }
  }

  libc_free(p);

  allocs = alloc_list_remove(allocs, p);
}
