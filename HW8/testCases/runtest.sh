#!/bin/bash
for test in $(seq -w 1 50)
do
	echo "Test: $test"
	g++ test.cc test_case_$test.cc hw8.a -o test
	./test
done
