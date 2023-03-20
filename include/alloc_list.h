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
 * alloc_list.h
 * Author: Alexandros Antonakakis <csd4802>
 * Created on: 18-03-2023
 * Description: Header file for alloc_list.c
 */

#ifndef ALLOC_LIST_H
#define ALLOC_LIST_H

#include <pthread.h>
#include <stddef.h>

typedef struct alloc_list {
  void              *ptr;  /* returned malloc address */
  size_t             size; /* size allocated */
  pthread_t          tid;  /* thread id */

  struct alloc_list *next;
} alloc_list;

extern alloc_list *allocs;

alloc_list *alloc_list_create(void);

alloc_list *alloc_list_add(alloc_list *list, void *ptr, size_t size);

alloc_list *alloc_list_remove(alloc_list *list, void *ptr);

void print_active_allocations(alloc_list *list);

void alloc_list_destroy(alloc_list *list);

int is_alloc_list_empty(alloc_list *list);

#endif
