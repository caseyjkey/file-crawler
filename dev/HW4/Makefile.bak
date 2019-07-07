CXXFLAGS =-std=c++11 -O -Wall -Wextra -Wpedantic

hw4: hw4.o Bunch.o Bunch.h
	$(CXX) $(CXXFLAGS) hw4.o Bunch.o -o $@

path.o:: Bunch.h
hw3.o:: hw4.cc Bunch.h

clean:
	rm -f main *.o
