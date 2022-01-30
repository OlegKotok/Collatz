COMPILER=g++
INCLUDE=thread-pool
OUTPUT=Collatz_OPTIMIZED

all: normal optimized

normal:
	gcc Collatz.c -o Collatz

optimized:
	$(COMPILER) -std=c++17 -I$(INCLUDE) Collatz_OPTIMIZED.cpp -o $(OUTPUT)
