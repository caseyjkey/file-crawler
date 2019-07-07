CXXOPTS=-Wall -Wextra -Wpedantic -fprofile-arcs -ftest-coverage
code: code.cc
	g++ $(CXXOPTS) $^ -o $@

clean:
	-rm -f code *.o *.gcov *.gcda *.gcno
