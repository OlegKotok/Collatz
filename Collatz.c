/*! \file    Collatz.c
 *  \brief   Simmple and fast solution to find longest Collatz sequence
 *  \author  Oleg Kotok
 *  \date    24.01.2022
 *  \version v_1
 *  \details Program one. Optimise for code simplicity and low memory usage. Implementation in clear C, without ++.
 *  \example gcc Collatz.c && time ./a.out 13
 */

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

/*! 
 *  \brief Get length of Collatz sequence
 *  \param [in] n Start number of Collatz sequence
 *  \return length of Collatz sequence for staring point n
 */
int Collatz(unsigned long long n)
{
    int i = 0;
    while (n > 1)
    {
        if (n % 2 == 0) /* n is even */
        {
            n >>= 1; /* n = n / 2 */
        }
        else  /* n is odd number */
        {
            n *= 3; /* n = n * 3 */
            n++;
        }
        i++;
    }
    return i;
}

/*! 
 *  \brief main function
 *  \param [in] argc {An integer that contains the count of arguments that follow in argv. The argc parameter is always greater than or equal to 1.}
 *  \param [in] argv {An array of null-terminated strings representing command-line arguments entered by the user of the program.}
 *  \details argv[1] have to contain ineger value - startiong position of Collatz sequence
 *  \return Return 0 if everything is ok
 *          -1 - invalid argument
 *          -2 - no arguments
 */
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        int basicSequence = atoi(argv[1]);
        if (basicSequence > 0) /* argv[1] is integer */
        {
            int maxLength = 0;
            int startingPositionForMaxSequence = 0;
            int currentLength = 0;

            while ( basicSequence > 0 )
            {
                currentLength = Collatz(basicSequence);
                if ( currentLength > maxLength )
                {
                    maxLength = currentLength;
                    startingPositionForMaxSequence = basicSequence;

                    printf("Basic number: %u \t Sequence length: %u \n",startingPositionForMaxSequence, maxLength);
                }
                --basicSequence;
            }

            return 0;
        }
        else
        {
             printf("%s is Invalid argument. Syntax: %s n - starting value of Collatz sequence \n", argv[1], argv[0]);
            return -1;
        }
    }
    else
    {
         printf("Syntax: %s n - starting value of Collatz sequence \n", argv[0]);
         return -2;
    }
}