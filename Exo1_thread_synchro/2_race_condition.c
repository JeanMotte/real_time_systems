#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int mails;

void *routine()
{
    for (int i = 0; i < 100000; i++)
    {
        mails++; // read mails, increment it, write it back
    }
}

// explanation of race condition
// two threads are executing the same code at the same time
// both read the value of mails at the same time
// both increment it
// both write it back
// mails is only incremented once instead of twice

int main(void *arg)
{
    pthread_t t1, t2; // pthread_t is a type for thread identifiers
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

    printf("Number of Mails: %d\n", mails);
    return 0;
}