#include <stdio.h>
#include <stdlib.h>

#include "alloc_list.h"

int main(void) {
    void *ptr = malloc(100);

    void *c = calloc(10, 400);

    void *f = realloc(0, 399);

    free(ptr);
    free(c);
    free(f);
    
    fprintf(stderr, "Active allocations after freeing everything:\n");

    print_active_allocations(allocs);
    
    ptr = malloc(100);
    c = calloc(10, 400);
    f = realloc(0, 399);


    fprintf(stderr, "\nActive allocations after leaking everything:\n");
    
    print_active_allocations(allocs);
}

