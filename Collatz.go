package main

import (
	"fmt"
	"os"
	"strconv"
)

var (
	maxLength int = 0
	startingPositionForMaxSequence  uint64 = 0;
)

func Collatz(n uint64) {
	var length int
	var startPosition = n
	for ; n > 1; length++ {
		if n % 2 == 0 /* n is even */ {
            n >>= 1; /* n = n / 2 */
        } else  /* n is odd number */ {
            n *= 3; /* n = n * 3 */
            n++;
        }
	}
    
    if length > maxLength {
        maxLength = length;
        startingPositionForMaxSequence = startPosition;

        fmt.Println("Basic number: ", startPosition, " \t Sequence length: ", length);
    }
}

func main() {
	fmt.Println("Go solution\n")
	
	if len (os.Args) == 1 {
        fmt.Println("Syntax: ", os.Args[0], "n - starting value of Collatz sequence \n")
        os.Exit(-2)
    }

	var basicSequence, err = strconv.Atoi (os.Args[1])
    if err != nil /* argv[1] is not integer */ {
        fmt.Println( os.Args[1], " is Invalid argument. Syntax: ", os.Args[0], " n - starting value of Collatz sequence \n" )
        os.Exit(-1)
    }

	for ; basicSequence > 0; basicSequence-- {
		Collatz (uint64 (basicSequence) )
	}
}