# malloc-allocation-tracker
A custom malloc wrapper that keeps track of allocations in an allocation list

## Why?

I was testing an application for memory leaks, and sadly I was not able to hook it to valgrind or the leak sanitizer
So I made my own

## Building
    - Run `make;make clean`
    - If all works out, the shared object `libmallocwrapper.so` will be created

## Using
    - You will have to include `alloc_list.h` in order to be able to access to allocation list
    - Compile your program with `cc -L. -lmallocwrapper` *(libmallocwrapper.so should be in the same folder)*
    - See the `test` directory for working examples

## Issues
    - While the implementation is thread-safe, it is **really** slow because of the global mutex lock
    - Instead of the regular libc `malloc`, `mmap` is used in the list functions

    
