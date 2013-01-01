all: case

case: case_test.cpp
	clang++ -std=c++11 case_test.cpp -g -o case_test
