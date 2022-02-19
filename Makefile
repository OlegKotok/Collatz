COMPILER=g++
INCLUDE=thread-pool
C_OUTPUT=Collatz1
CPP_OUTPUT=Collatz2
MULTITHREADED_C_OUTPUT=Collatz3
MULTITHREADED_CPP_OUTPUT=Collatz4
TEST_VALUE=10000000

all: normal multithreaded optimized optimized_mt 

normal:
	gcc -O3 -Wall Collatz.c -o $(C_OUTPUT)

multithreaded:
	gcc -O3 -Wall Collatz_MT.c -o $(MULTITHREADED_C_OUTPUT) -lpthread

optimized:
	$(COMPILER) -std=c++2a -O3 -Wall Collatz.cpp -o $(CPP_OUTPUT)

optimized_mt:
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

test-multithreaded:
	time ./$(MULTITHREADED_C_OUTPUT) $(TEST_VALUE)

test-multithreaded-cpp:
	time ./$(MULTITHREADED_CPP_OUTPUT) $(TEST_VALUE)

test-go:
	time go run Collatz.go $(TEST_VALUE)

test: test-normal  test-multithreaded test-cpp test-multithreaded-cpp test-go

docs:
	doxygen doxygen.conf