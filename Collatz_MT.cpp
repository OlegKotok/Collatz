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
#include <map>
#include <mutex>

constexpr int threadTaskLimit = 100;

std::mutex mtx;

/*! 
 *  \brief Get length of Collatz sequence
 *  \param [in] n Start number of Collatz sequence
 *  \param [in] cachedStorage Link to std::map<unsigned long long, int> which storage previously-calculated values, start position and length of sequence for this position
 *  \return std::pair startPositin and length of Collatz sequence for staring point n
 */
auto Collatz(unsigned long long startPositin, std::map<unsigned long long, int> &cachedStorage)
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
    mtx.lock();
    for (auto currentPosition : collatzSequence)
    {
        cachedStorage.insert( std::make_pair(currentPosition, currentLength) );
        currentLength--;    
    }
    mtx.unlock();

    return std::make_pair (startPositin, length);
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

    if (argc > 1)
    {
        int startPosition = atoi(argv[1]);
        if (startPosition > 0) /* argv[1] is integer */
        {
            int maxLength = 0;
            int startingPositionForMaxSequence = 0;
            int currentStartingPosition = 0;
            int currentLength = 0;
            std::map<unsigned long long, int> cachedCollatzSequenceStorage;

            thread_pool pool; /** Constructs a thread pool with as many threads as available in the hardware. */
            std::queue <std::future <std::pair <unsigned long long, int> > > futureStorage; /** Contains futures objects from Collatz thread-pool */

            while ( startPosition > 0 )
            {
                futureStorage.push (pool.submit(Collatz, startPosition, std::ref(cachedCollatzSequenceStorage) ) );
                startPosition--;

                if (pool.get_tasks_queued() > threadTaskLimit)
                {
                    /** wating thred-pool finishing. good time to process futureStorage **/
                    while (!futureStorage.empty())
                    {
                        auto p = futureStorage.front().get();
                        currentStartingPosition = p.first;
                        currentLength = p.second;
                        if ( currentLength > maxLength ) {
                            maxLength = currentLength;
                            startingPositionForMaxSequence = currentStartingPosition;

                            std::cout <<"Basic number: " << startingPositionForMaxSequence << " \t Sequence length: " << maxLength << std::endl;
                        }
                        futureStorage.pop();
                    }

                     /** wating thred-pool finishing if don't finished yet **/
                    if (pool.get_tasks_queued() > pool.get_thread_count())
                    {
                        pool.wait_for_tasks();
                    }
                }
            }
            pool.wait_for_tasks();

            // ** find max lenth in sequence ** //
            while (!futureStorage.empty())
            {
                auto p = futureStorage.front().get();
                currentStartingPosition = p.first;
                currentLength = p.second;
                if ( currentLength > maxLength ) {
                    maxLength = currentLength;
                    startingPositionForMaxSequence = currentStartingPosition;

                    std::cout <<"Basic number: " << startingPositionForMaxSequence << " \t Sequence length: " << maxLength << std::endl;
                }
                futureStorage.pop();
            }
            
            std::cout << std::endl;
            std::cout << "Basic number: " << startingPositionForMaxSequence << std::endl;
            std::cout << "Sequence length: " << maxLength <<std::endl;
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