#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;
int lock = 0;

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
    pthread_t th[4];
    int i;
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 4; i++)
    {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
        {
            return 1;
        }
        printf("Thread %d created\n", i);
    }

    for (i = 0; i < 4; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            return 2;
        };
        printf("Thread %d joined\n", i);
    }
    pthread_mutex_destroy(&mutex);
    printf("Number of Mails: %d\n", mails);
    return 0;
}