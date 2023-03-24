# malloc-allocation-tracker
A custom malloc wrapper that keeps track of allocations in an allocation list

## Why?

I was testing an application for memory leaks, and sadly I was not able to hook it to valgrind or the leak sanitizer
So I made my own

## Building
* Run `make`
* If all works out, the shared object `libmallocwrap.a` will be created

## Using
* You will have to include `alloc_list.h` in order to be able to access to allocation list
* Compile your program with `cc yourcode.c libmallocwrap.a -ldl` *Make sure to link -ldl*
* See the `test` directory for a working example

## Issues
* The list functions are not thread safe
* The list delete function is very slow since it has to iterate through the list
* Instead of the regular libc `malloc`, `mmap` is used in the list functions

    
