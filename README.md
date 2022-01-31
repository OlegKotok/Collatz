# Collatz sequence

## Problem description
Write two programs that find the starting number below a given limit that has the longest Collatz sequence. The program should be called with a single number as the command line parameter. This number is the upper limit below which the longest chain should be found. Note that intermediate steps may go above this upper limit. Program one will optimize for code simplicity and low memory usage. Program two sacrifices some simplicity and memory for a significant reduction in CPU time required.

### Collatz sequence definition

The following iterative sequence is defined for the set of positive integers:

> n → n/2 (n is even)

> n → 3n + 1 (n is odd)

Using the rule above and starting with 13, we generate the following sequence:

> 13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1

It can be seen that this sequence (starting at 13 and finishing at 1) contains 10 terms. Although it has not been proved yet (Collatz Problem), it is thought that all starting numbers finish at 1.

### Marking criteria
The code must be C or C++. Follow best practices of the chosen language. Code must run without issues on a standard Linux system. Any non-standard configuration or library usage must be noted. Make notes of any restrictions on program usage. Correctness, maintainability, and completeness of the solution are important.

## Solution

### Files
 - Collatz.c - Simmple and fast solution writed in clean C optimise for code simplicity and low memory usage
 - Collatz.cpp - Optimise by storing already calculated value in std::map container. Single-thread, writen in C++.
 - Collatz_MT.cpp - Multithreded thread-pool optimization. Using thread-pool, futures, mutex, using a lot of memory and and contain a lot of code.

### The idea
The idea was determine Collatz sequence calculation between few threads. We can't create billions of threads in your home pc, so the second idea was use thread pools for it. And, also, it looks like good idea to store already calculated value in std::map container. So, if this value is already calculated, we don't need to calculate all sequences.

### How to run
I'm using gcc compiler under WSL on Windows. Try install it visit https://docs.microsoft.com/en-us/windows/wsl/install
In wsl (**Win+R** and type _bash_) mode write _'sudo apt install build-essential'_. For more info visit https://linuxize.com/post/how-to-install-gcc-compiler-on-ubuntu-18-04/

To compile programs write _'make all'_

Don't forget about submodules:
 - git submodule init
 - git submodule update

### Result
The first program, more simple solution was more faster. To write the first program was spent maximum 20 minutes, for the second one - few hours, but first one program comes not only more simple and more faster. **KISS - Keep it simple, stupid!**
