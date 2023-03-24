#  MIT License

# Copyright (c) 2023 Alexandros Emmanouil Antonakakis <antonakakis1@gmail.com>

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# Author: Alexandros Antonakakis
# Date: 20-03-2023
# Description: Makefile for the malloc wrapper library

CC := gcc
CFLAGS := -Wall -I./include

SRC := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)

.PHONY: all clean check build compile

all: check build clean

check:
ifeq ($(CC), 0)
	$(error gcc not found)
endif

build: compile
	ar -rcs libmallocwrap.a *.o

compile: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f malloc_wrapper.o alloc_list.o
