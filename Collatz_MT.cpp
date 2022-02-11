/*! \file    Collatz_MT.cpp
 *  \brief   OptimMultithreaded solution to find longest Collatz sequence
 *  \author  Oleg Kotok
 *  \date    31.01.2022
 *  \version v_2.2
 *  \details Program two. Multithreading optimization. Sacrifices some simplicity and memory for significant reduction in CPU time required.
 *  \example make test
 *  \todo    Make cache already-calculated Collatz sequence
 */

#include <iostream>
#include <stdlib.h>
#include <thread>
#include "thread_pool.hpp"
#include <future>
#include <queue>
#include <list>
#include <unordered_map>
#include <mutex>

constexpr int threadTaskLimit = 100;

std::mutex mtx;

/*! 
 *  \brief Get length of Collatz sequence
 *  \param [in] startPosition Start number of Collatz sequence
 *  \param [in] cachedStorage Link to std::map<unsigned long long, int> which storage previously-calculated values, start position and length of sequence for this position
 *  \param [out] maxLength Link to max length, this value will be ipdated if this Collatz sequence logest
 *  \param [out] startingPositionForMaxSequence Staring position for max length sequence. Ref value
 *  \return void
 */
void Collatz(unsigned long long startPosition, std::unordered_map<unsigned long long, int> &cachedStorage, int &maxLength, unsigned long long &startingPositionForMaxSequence)
{
    int length = 0;
    auto value = startPosition;
    std::list<unsigned long long> collatzSequence;

    /** calculate value if not in cashe */
    while (value > 1 && !cachedStorage.contains(value))
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

    mtx.lock();
    /** get length if cashed */
    if (cachedStorage.contains(value))
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

    /** update maxLength */
    if ( length > maxLength ) {
        maxLength = length;
        startingPositionForMaxSequence = startPosition;

        std::cout <<"Basic number: " << startPosition << " \t Sequence length: " << maxLength << std::endl;
    }
    mtx.unlock();
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
    std::cout << "Thread pool library version is " << THREAD_POOL_VERSION << std::endl <<std::endl;

    if (argc == 1)
    {
        std::cout << "Syntax: " << argv[0] << " <n> - starting value of Collatz sequence" <<std::endl;
        return -2;
    }
   
    int startPosition = atoi(argv[1]);
    if (startPosition == 0) /* argv[1] is not integer */
    {
        std::cout << argv[1] << " is Invalid argument." << std::endl;
        std::cout << "Syntax: " << argv[0] << " <n> - starting value of Collatz sequence" <<std::endl;
        return -1;
    }

    int maxLength = 0;
    unsigned long long startPositionForMaxLength = 0;
    std::unordered_map<unsigned long long, int> cachedCollatzSequenceStorage;

    thread_pool pool; /** Constructs a thread pool with as many threads as available in the hardware. */
    std::queue <std::future <std::pair <unsigned long long, int> > > futureStorage; /** Contains futures objects from Collatz thread-pool */

    while ( startPosition > 0 )
    {
        pool.push_task( Collatz, startPosition, std::ref(cachedCollatzSequenceStorage),  std::ref(maxLength), std::ref(startPositionForMaxLength) );
        startPosition--;
    }
    
    pool.wait_for_tasks();
            
    std::cout << std::endl;
    std::cout << "Basic number: " << startPositionForMaxLength << std::endl;
    std::cout << "Sequence length: " << maxLength <<std::endl;
    
    return 0;
}