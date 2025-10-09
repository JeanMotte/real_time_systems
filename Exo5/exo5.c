#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define M 3

int AIR[N];
int SOL[M];

int count_air = 0, count_sol = 0;

#define P(sem) sem_wait(&sem)
#define V(sem) sem_post(&sem)

pthread_mutex_t mutex_piste;

sem_t solVide, solPlein, airVide, airPlein;

void *ajoutSol(void *arg)
{
    while (1)
    {
        P(solVide);
        SOL[count_sol] = 1;
        count_sol++;
        V(solPlein);
    }
}

void *approcheAir(void *arg)
{
    while (1)
    {
        P(airVide);
        AIR[count_air] = 1;
        count_air++;
        V(airPlein);
    }
}

void *atterrir(void *arg)
{
    while (1)
    {

        P(airVide);
        pthread_mutex_lock(&mutex_piste);

        int avion_atterri = AIR[0];
        for (int i = 0; i < count_air - 1; i++)
        {
            AIR[i] = AIR[i + 1];
        }
        count_air++;
        pthread_mutex_unlock(&mutex_piste);
        V(airPlein);
    }
    return NULL;
}

void *decoller(void *arg)
{
    while (1)
    {
        P(solPlein);
        pthread_mutex_lock(&mutex_piste);

        // Fait décoller l'avion en tête de la zone Sol
        int avion_decolle = SOL[0];
        for (int i = 0; i < count_sol - 1; i++)
        {
            SOL[i] = SOL[i + 1];
        }
        SOL[count_sol] = 0;
        count_sol--;
        
        
        pthread_mutex_unlock(&mutex_piste);
        V(solVide);
    }
    return NULL;
}

int main()
{
    pthread_t t_SortirAvion, t_Decollage, t_ApprocheAVion, t_Atterrissage;
    pthread_mutex_init(&mutex_piste, NULL);

    sem_init(&solVide, 0, M);
    sem_init(&solPlein, 0, 0);
    sem_init(&airVide, 0, N);
    sem_init(&airPlein, 0, 0);

    pthread_create(&t_SortirAvion, NULL, ajoutSol, NULL);
    pthread_create(&t_Decollage, NULL, decoller, NULL);
    pthread_create(&t_ApprocheAVion, NULL, approcheAir, NULL);
    pthread_create(&t_Atterrissage, NULL, atterrir, NULL);

    pthread_join(t_SortirAvion, NULL);
    pthread_join(t_Decollage, NULL);
    pthread_join(t_ApprocheAVion, NULL);
    pthread_join(t_Atterrissage, NULL);

    pthread_mutex_destroy(&mutex_piste);
    sem_destroy(&solVide);
    sem_destroy(&solPlein);
    sem_destroy(&airVide);
    sem_destroy(&airPlein);

    return 0;
}