#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *routine1()
{
    printf("Test from thread1\n");
    sleep(1);
    printf("End of thread1\n");
}

void *routine2()
{
    printf("Test from thread2\n");
    sleep(1);
    printf("End of thread2\n");
}

int main(void *arg)
{
    pthread_t t1, t2;                           // pthread_t is a type for thread identifiers
    pthread_create(&t1, NULL, &routine1, NULL); // create a thread that will execute the routine function
    pthread_create(&t2, NULL, &routine2, NULL);

    pthread_join(t1, NULL); // wait for thread t1 to finish.
    pthread_join(t2, NULL);
    // We can assign the return value of the thread to a variable instead of NULL

    return 0;
}