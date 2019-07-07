CXXFLAGS := -g -Wall -Wextra -Wpedantic -Werror -Wfatal-errors

default: dir-simple dir-object

dir-object: dir-object.cc Directory.h Directory.cc
	$(CXX) $(CXXFLAGS) -o $@ dir-object.cc Directory.cc

clean:
	rm -f a.out dir-simple dir-object *.o *.gch *.gcov *.gcda *.gcno core
