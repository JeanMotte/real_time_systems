#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;
int lock = 0;

// mutex allows to lock a section of code so that only one thread can access it at a time
// setting its value to 0 means it's unlocked
// setting its value to 1 means it's locked
pthread_mutex_t mutex;

void *routine()
{
    for (int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);   // lock the mutex before accessing the shared resource
        mails++;                      // read mails, increment it, write it back
        pthread_mutex_unlock(&mutex); // unlock the mutex after accessing the shared resource
    }
}

int main(void *arg)
{
    pthread_t t1, t2; // pthread_t is a type for thread identifiers

    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&t1, NULL, &routine, NULL) != 0)
    { // create a thread that will execute the routine function
        return 1;
    }
    if (pthread_create(&t2, NULL, &routine, NULL) != 0)
    {
        return 2;
    }

    if (pthread_join(t1, NULL) != 0)
    {
        return 3;
    }; // wait for thread t1 to finish.
    if (pthread_join(t2, NULL) != 0)
    {
        return 4;
    };
    // We can assign the return value of the thread to a variable instead of NULL

    pthread_mutex_destroy(&mutex);
    printf("Number of Mails: %d\n", mails);
    return 0;
}