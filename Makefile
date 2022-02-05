COMPILER=g++
INCLUDE=thread-pool
C_OUTPUT=Collatz1
CPP_OUTPUT=Collatz2
MULTITHREADED_OUTPUT=Collatz3
TEST_VALUE=1899148184679

all: normal optimized multithreaded

normal:
	gcc -O3 -Wall Collatz.c -o $(C_OUTPUT)

optimized:
	$(COMPILER) -O3 -Wall Collatz.cpp -o $(CPP_OUTPUT)

multithreaded:
	$(COMPILER) -std=c++17 -O3 -Wall -I$(INCLUDE) Collatz_MT.cpp -o $(MULTITHREADED_OUTPUT) -lpthread

clean:
	rm $(C_OUTPUT)
	rm $(CPP_OUTPUT)
	rm $(MULTITHREADED_OUTPUT)

test: all
	time ./$(C_OUTPUT) $(TEST_VALUE)
	time ./$(CPP_OUTPUT) $(TEST_VALUE)
	time ./$(MULTITHREADED_OUTPUT) $(TEST_VALUE)

docs:
	doxygen doxygen.conf