#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BARRIER_NUM 5

sem_t semaphore;

void *routine(void *arg)
{
    sem_wait(&semaphore);
    sleep(1);
    printf("hello from thread %d\n", *(int *)arg);
    sem_post(&semaphore);
    free(arg);
}

int main()
{
    pthread_t th[BARRIER_NUM];
    sem_init(&semaphore, 0, 1);
    int i;

    for (i = 0; i < BARRIER_NUM; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0)
        {
            perror("error on pthread_create");
        }
    }

    for (i = 0; i < BARRIER_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("error on pthread_join");
        }
    }

    sem_destroy(&semaphore);
    return 0;
}