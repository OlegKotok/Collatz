COMPILER=g++
INCLUDE=thread-pool
C_OUTPUT=Collatz_normal
CPP_OUTPUT=Collatz_cpp
MULTITHREADED_C_OUTPUT=Collatz_mt
MULTITHREADED_CPP_OUTPUT=Collatz_mt_cpp
TEST_VALUE=10000000

all: normal mt cpp cpp-mt 

normal:
	gcc -O3 -Wall Collatz.c -o $(C_OUTPUT)

mt:
	gcc -O3 -Wall Collatz_MT.c -o $(MULTITHREADED_C_OUTPUT) -lpthread

cpp:
	$(COMPILER) -std=c++2a -O3 -Wall Collatz.cpp -o $(CPP_OUTPUT)

cpp-mt:
	$(COMPILER) -std=c++2a -O3 -Wall -I$(INCLUDE) Collatz_MT.cpp -o $(MULTITHREADED_CPP_OUTPUT) -lpthread

clean:
	rm $(C_OUTPUT)
	rm $(CPP_OUTPUT)
	rm $(MULTITHREADED_C_OUTPUT)
	rm $(MULTITHREADED_CPP_OUTPUT)

test-normal:
	time ./$(C_OUTPUT) $(TEST_VALUE)

test-cpp:
	time ./$(CPP_OUTPUT) $(TEST_VALUE)

test-mt:
	time ./$(MULTITHREADED_C_OUTPUT) $(TEST_VALUE)

test-cpp-mt:
	time ./$(MULTITHREADED_CPP_OUTPUT) $(TEST_VALUE)

test-go:
	time go run Collatz.go $(TEST_VALUE)

test: test-normal  test-mt test-cpp test-go

docs:
	doxygen doxygen.conf