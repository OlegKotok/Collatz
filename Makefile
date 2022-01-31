COMPILER=g++
INCLUDE=thread-pool
C_OUTPUT=Collatz1
CPP_OUTPUT=Collatz2

all: normal optimized

normal:
	gcc -O3 -Wall Collatz.c -o $(C_OUTPUT)

optimized:
	$(COMPILER) -std=c++17 -O3 -Wall -I$(INCLUDE) Collatz_OPTIMIZED.cpp -o $(CPP_OUTPUT) -lpthread 

clean:
	rm $(C_OUTPUT)
	rm $(CPP_OUTPUT)

test: all
	time ./$(C_OUTPUT) 1899148184679
	time ./$(CPP_OUTPUT) 1899148184679