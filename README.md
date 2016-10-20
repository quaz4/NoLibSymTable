# NoLibSymTable
A symbol table implemented using either library calls or system call.
Written by William Stewart

# Building
There are two versions you can build, the one implemented using library calls and
the one without. The commands for both are listed below:

Library version:    	make
System call version:	make sys

# Running
To run the test application, use the following command:
	./Test

# Implemented Features
I chose to implement the java hashCode hash function and used linear probing to resolve collisions.

# Replaced Library Calls
malloc() I replaced with mmap()
free() I replaced with munmap()
print() I replaced with write()

# Test program
I implemented a simple address book program. All functions of the Symbol Table can be tested.
If

# Faults
There are currently no known bugs in the program. There were previously issues with malloc
and #ifdef blocks but those issues were resolved.
