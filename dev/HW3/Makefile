CXXFLAGS =-std=c++11 -O -Wall -Wextra -Wpedantic

hw3: hw3.o path.o path.h
	$(CXX) $(CXXFLAGS) hw3.o path.o -o $@

path.o:: path.h
hw3.o:: hw3.cc path.h

clean:
	rm -f main *.o
