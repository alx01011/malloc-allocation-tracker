# Author: Alexandros Antonakakis
# Date: 20-03-2023
# Description: Makefile for example programs of the malloc wrapper library

CC := gcc
CFLAGS := -I../include
LDFLAGS := -ldl

# gcc parallel.c libmalloc.a
.PHONY: sequential clean

sequential: sequential.c libmallocwrap.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< libmallocwrap.a

clean:
	rm -f *.o *.so parallel sequential

