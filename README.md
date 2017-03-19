# MVECTOR

## Purpose
An easy way to allocate / resize / access / deallocate memory of basic 
types, using a syntax-like std::vector, but with a bit different functions 
operations.

I deciced to create this library because in some embedded platforms I 
noticed that std::vector memory allocations had undesirable results (i.e. 
aliased memory allocated regions(!)).

In this library I'm using standard malloc/free operations.

This library is under development. It may not work correct!