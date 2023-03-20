# Author: Alexandros Antonakakis
# Date: 20-03-2023
# Description: Makefile for example programs of the malloc wrapper library

CC := gcc
CFLAGS := -I../include
LDFLAGS := -L. -pthread

# gcc parallel.c -pthread -L. -lmallocwrapper

.PHONY: parallel sequential clean

parallel: parallel.c libmallocwrapper.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -lmallocwrapper

sequential: sequential.c libmallocwrapper.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -lmallocwrapper

clean:
	rm -f *.o *.so parallel sequential

