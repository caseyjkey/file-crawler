CXXFLAGS =-std=c++11 -O -ggdb -Wall -Wextra -Wpedantic

hw7(Bunch.o): Bunch.o Fing.o
	ar cr hw8.a Bunch.o
	ar cr hw8.a Fing.o
clean:
	rm -f main *.o
