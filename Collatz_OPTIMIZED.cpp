/*! \file    Collatz.c
 *  \brief   Simmple and fast solution to find longest Collatz sequence
 *  \author  Oleg Kotok
 *  \date    24.01.2022
 *  \version v_2
 *  \details Program two. Sacrifices some simplicity and memory for significant reduction in CPU time required.
 *  \example gcc Collatz.c && time ./a.out 13
 *  \todo    Implement ThreadPoool
 */

#include <iostream>
#include <stdlib.h>
#include <thread>

/*! 
 *  \brief Get length of Collatz sequence
 *  \param [in] n Start number of Collatz sequence
 *  \return length of Collatz sequence for staring point n
 */
int Collatz(unsigned long long n)
{
    int i = 1;
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
    std::cout << "C++ language standard: " << __cplusplus << std::endl; /* cpp version */
    std::cout << "Thread count: " << std::thread::hardware_concurrency() << std::endl;

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

                    std::cout << "Basic number: " << startingPositionForMaxSequence << "\t";
                    std::cout << "Sequence length: " << maxLength <<std::endl;
                }
                --basicSequence;
            }

            return 0;
        }
        else
        {
            std::cout << argv[1] << " is Invalid argument." << std::endl;
            std::cout << "Syntax: " << argv[0] << " n - starting value of Collatz sequence" <<std::endl;
            return -1;
        }
    }
    else
    {
         std::cout << "Syntax: " << argv[0] << " n - starting value of Collatz sequence" <<std::endl;
         return -2;
    }
}