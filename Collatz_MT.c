/*! \file    Collatz.c
 *  \brief   Simmple and fast solution to find longest Collatz sequence
 *  \author  Oleg Kotok
 *  \date    11.02.2022
 *  \version v_2
 *  \details Multithreaded optimization with posix threads
 *  \example make test-multithreaded TEST_VALUE=10000000 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREAD_COUNT 8
#define STEP_SIZE 2000
#define DEBUG 1

pthread_mutex_t mutex;

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

/** Struct to store parameters for thread function. Posix thread doesn't support multiple parameters in thread functions*/
struct Results
{
    int currentstartPosition;           /** Current start number of Collatz sequence. Value will be updated after calculation */
    int maxLength;                      /** Max length, this value will be updated if this Collatz sequence logest */
    int startingPositionForMaxSequence; /** Staring position for max length sequence. Ref value */
};

/*! 
 *  \brief Get length of Collatz sequence and update max length
 *  \param args Pointer to Results struct which store parameters for thread function
 *  \see Results
 *  \return 0
 */
void* updateMaxCollatzSequence(void *args)
{
    struct Results *p = (struct Results*) args;
    while ( p->currentstartPosition > 0 )
    {
        pthread_mutex_lock(&mutex);
        //update start position for next thread
        int startPosition = p->currentstartPosition;
        int endPosition = p->currentstartPosition - STEP_SIZE;
        if (endPosition < 0) endPosition = 0;
        p->currentstartPosition = endPosition;
        pthread_mutex_unlock(&mutex);

        //arrays to store bufer data
        int position[STEP_SIZE];
        int len[STEP_SIZE];
        int index = 0;

        //fill array - calculation
        while (index < STEP_SIZE && startPosition > endPosition) {
            position[index] = startPosition;
            len[index] = Collatz(startPosition);
            index++;
            startPosition--;
        } // index - length of the array

        //update max Collatz Sequence
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < index; i++)
        {
            if ( len[i] > p->maxLength )
            {
                p->maxLength = len[i];
                p->startingPositionForMaxSequence = position[i];

                if (DEBUG) printf("Basic number: %u \t Sequence length: %u \n", p->startingPositionForMaxSequence, p->maxLength);
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
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
    printf("C-style multi-threaded optimization\nSolution with mutex\n\n");
    
    if (argc == 1)
    {
        printf("Syntax: %s n - starting value of Collatz sequence \n", argv[0]);
        return -2;
    }
   
    int basicSequence = atoi(argv[1]);
    if (basicSequence == 0) /* argv[1] is not integer */
    {
        printf("%s is Invalid argument. Syntax: %s n - starting value of Collatz sequence \n", argv[1], argv[0]);
        return -1;
    }

    /** struct to store parameters for thread function */
    struct Results s = {.maxLength = 0, .startingPositionForMaxSequence = 0, .currentstartPosition = basicSequence};

    pthread_t threads[THREAD_COUNT];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        int status = pthread_create(&threads[i], NULL, updateMaxCollatzSequence, (void*) &s);
        if (status != 0)
        {
            printf("main error: can't create thread, status = %d\n", status);
            exit(status);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        int status = pthread_join(threads[i], NULL);
        if (status != 0)
        {
            printf("main error: can't join thread, status = %d\n", status);
            exit(status);
        }
    }
    pthread_mutex_destroy(&mutex);

    printf("\nBasic number: %u \t Sequence length: %u \n\n", s.startingPositionForMaxSequence, s.maxLength);
    return 0;
}