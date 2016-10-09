# NoLibSymTable
A symbol table implemented using either library calls or system call.
Written by William Stewart

# Makefile
To build library version run "make"
To build system call version run "make sys"

# Implemented features
I implemented all features from the specifications and they all work.

# Faults
My program has one fault, I had a strange problem trying to to build the library version
into the sytem call verion, if the malloc or mmap call was inside an #ifdef block in the
stringCopy function, the program would seg fault. So I only implemented saving the key
using mmap as the malloc version was only to make marking easy.
