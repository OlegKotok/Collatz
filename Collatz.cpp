/*! \file    Collatz.cpp
 *  \brief   Simmple and fast solution to find longest Collatz sequence with optimization
 *  \author  Oleg Kotok
 *  \date    31.01.2022
 *  \version v_2
 *  \details Program two. Sacrifices some simplicity and memory for significant reduction in CPU time required.
 *  \example make optimized
 */

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <map>

/*! 
 *  \brief Get length of Collatz sequence
 *  \param [in] n Start number of Collatz sequence
 *  \param [in] cachedStorage Link to std::map<unsigned long long, int> which storage previously-calculated values, start position and length of sequence for this position
 *  \return length of Collatz sequence for staring point n
 */
int Collatz(unsigned long long startPositin, std::map<unsigned long long, int> &cachedStorage)
{
    int length = 0;
    auto value = startPositin;
    std::list<unsigned long long> collatzSequence;

    /** calculate value if not in cashe */
    while (value > 1 && cachedStorage.count(value) == 0)
    {
        collatzSequence.push_back(value); /** push to temp storage */

        if (value % 2 == 0) /* n is even */
        {
            value >>= 1; /* n = n / 2 */
        }
        else  /* n is odd number */
        {
            value *= 3; /* n = n * 3 */
            ++value;
        }
        length++;
    }

     /** get length if cashed */
    if (cachedStorage.count(value) > 0)
    {
        length += cachedStorage.find(value)->second; /** get length for stored key-value */
    }

    /** push newly-generated Collatz sequence (if exist) to cachedStorage */
    auto currentLength = length;
    for (auto currentPosition : collatzSequence)
    {
        cachedStorage.insert( std::make_pair(currentPosition, currentLength) );
        currentLength--;    
    }

    return length;
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
    printf("C++ single-thread solution with caching-value optimization\n\n");
    if (argc > 1)
    {
        int basicSequence = atoi(argv[1]);
        if (basicSequence > 0) /* argv[1] is integer */
        {
            int maxLength = 0;
            int startingPositionForMaxSequence = 0;
            int currentLength = 0;
            std::map<unsigned long long, int> cachedCollatzSequenceStorage;

            while ( basicSequence > 0 )
            {
                currentLength = Collatz(basicSequence, cachedCollatzSequenceStorage);
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