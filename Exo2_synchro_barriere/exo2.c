#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // sleep, usleep
#include <time.h>   // srand, rand

#define N 5 // nombre de threads dans le groupe

pthread_mutex_t lock;
pthread_cond_t cond;

int arrived = 0; // compteur de threads arrivés

void *thread_groupe(void *arg)
{
    int id = *(int *)arg;
    free(arg); // on libère la mémoire allouée pour l'id

    // travail avant le point de rdv
    usleep((rand() % 1000) * 1000);
    printf("[Thread %d] Avant le point de rdv\n", id);

    // point de rdv
    pthread_mutex_lock(&lock);
    arrived++;
    printf("[Thread %d] A atteint le point de rdv (%d/%d)\n", id, arrived, N);

    if (arrived == N)
    {
        // dernier thread : il réveille le thread_barriere
        printf("[Thread %d] Je suis le dernier, je réveille le thread_barriere\n", id);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&lock);

    // travail après le point de rdv
    usleep((rand() % 1000) * 1000);
    printf("[Thread %d] Après le point de rdv\n", id);

    return NULL;
}

void *thread_barriere(void *arg)
{
    // travail avant la barrière
    usleep((rand() % 1000) * 1000);
    printf("[Thread B] Arrivé à la barrière, j’attends les %d threads\n", N);

    // attend que tous les threads soient arrivés
    pthread_mutex_lock(&lock);
    while (arrived < N)
    {
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);

    printf("[Thread B] Tous les threads sont arrivés, je continue\n");

    // travail après la barrière
    usleep((rand() % 1000) * 1000);
    printf("[Thread B] Après la barrière\n");

    return NULL;
}

int main()
{
    srand(time(NULL));

    pthread_t threads[N];
    pthread_t barrier;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // création du thread barrière
    pthread_create(&barrier, NULL, thread_barriere, NULL);

    // création des N threads du groupe
    for (int i = 0; i < N; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, thread_groupe, id);
    }

    // attente de fin des threads
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_join(barrier, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
