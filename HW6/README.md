To find my segfault in minus, Jack directed me to do this:
1. Add -ggdb flag to my Makefile
2. Compile test.cc and hw6.a by running:
* g++ -Wall -ggdb test.cc hw6.a
3. Ran gdb.a.out
* Typed run
* After encountering the segfault, we ran bt (not tb)
* Using the bcktrace, we observed the top of the stack (0. Stuff)
* We followed down the stack until we noticed line 67 of erase.

Notes: Weird stuff, like reading uninitialized memory, results in weird effects.

