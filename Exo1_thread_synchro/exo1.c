#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define P(sem) sem_wait(&sem) // macro du préprocesseur. Avant compilation, chaque P(x) est remplacé par sem_wait(&x).
#define V(sem) sem_post(&sem)

sem_t sem1, sem2, sem3, sem4; // sémaphore POSIX, declare them so that main and threads can access them

void *thread_1_synchro(void *arg) // pthread requires signature
{
    P(sem1);
    printf("Je sais ");
    V(sem2);

    P(sem3);
    printf("ce type ");
    V(sem4);

    return NULL;
}

void *thread_2_synchro(void *arg)
{
    P(sem2);
    printf("toujours faire ");
    V(sem3);

    P(sem4);
    printf("de synchronisation !\n");

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    sem_init(&sem1, 0, 1); // thread_1 peut démarrer
    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 0);
    sem_init(&sem4, 0, 0);

    pthread_create(&t1, NULL, thread_1_synchro, NULL);
    pthread_create(&t2, NULL, thread_2_synchro, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
    sem_destroy(&sem4);

    return 0;
}
