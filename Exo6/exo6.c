#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NOMBRE_THREAD_GROUPE 10
#define TEMPS_ATTENTE 5
#define NOMBRE_BOUCLES_BARRIERES 5

pthread_t listeThreadsGroupe[NOMBRE_THREAD_GROUPE];

pthread_mutex_t mutex;
sem_t semaphoreBarriere;
int nombreThreadsDansBarriere = 0;

int genererNombreAleatoire(int borneMax)
{
    // on génère un nombre aléatoire compris entre 0 et borneMax inclus
    return (rand() % (borneMax + 1));
}

void interrompteProgramme(int tempsInterruption)
{
    // on interrompt le programme pendant n secondes;
    sleep(tempsInterruption);
}

void barriere(int numThread)
{
    pthread_mutex_lock(&mutex);
    // on incrémente le nombre de threads ayant atteint la barrière
    nombreThreadsDansBarriere += 1;
    printf("[Thread %d] en attente ...\n", numThread);
    pthread_mutex_unlock(&mutex);

    // si c'est le dernier thread qui arrive, on libère une ressource du sémaphore
    if (nombreThreadsDansBarriere == NOMBRE_THREAD_GROUPE)
    {
        pthread_mutex_lock(&mutex);
        nombreThreadsDansBarriere = 0; // reset pour la prochaine barrière
        pthread_mutex_unlock(&mutex);

        for (int i = 0; i < NOMBRE_THREAD_GROUPE; i++)
        {
            sem_post(&semaphoreBarriere);
        }

        // le problème s'il y a plusieurs boucles et que lors de la libération du sémaphore, les threads appelle barriere dans la deuxième boucle
        // et il se peut que pendant la libération des sémaphores, la deuxième boucle arrive passe dans le else car pas bloquée car le sémaphore est > 0, et atteint la barrière directement
    }
    else
    {
        // sinon on réserve une ressource
        sem_wait(&semaphoreBarriere);

        printf("[Thread %d] Barriere atteinte par tous les threads, reprise de l'exécution du thread ...\n", numThread);
    }

    printf("[Thread %d] termine\n", numThread);
}

void *thread_barriere(void *numThread)
{
    int indiceThread = *((int *)numThread);

    for (int i = 0; i < NOMBRE_BOUCLES_BARRIERES; i++)
    {
        interrompteProgramme(genererNombreAleatoire(TEMPS_ATTENTE));
        printf("[Thread  %d], boucle %d\n", indiceThread + i, i);
        barriere(indiceThread);
    }

    return NULL;
}

int main()
{
    srand(clock());

    // on créé les mutex et sémaphores
    if (sem_init(&semaphoreBarriere, 0, 0))
    {
        perror("Impossible d'initialiser les semaphores.\n");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);

    // création des threads
    for (int i = 0; i < NOMBRE_THREAD_GROUPE; i++)
    {
        int *indice = malloc(sizeof(int));
        *indice = i;
        pthread_create(&listeThreadsGroupe[i], NULL, thread_barriere, (void *)indice);
    }

    // on attend que tous les threads soient terminés
    for (int i = 0; i < NOMBRE_THREAD_GROUPE; i++)
    {
        pthread_join(listeThreadsGroupe[i], NULL);
    }

    // on détruit le sémaphore
    sem_destroy(&semaphoreBarriere);

    return EXIT_SUCCESS;
}