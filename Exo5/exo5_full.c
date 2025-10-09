#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define M 3

#define P(sem) sem_wait(&sem)
#define V(sem) sem_post(&sem)

int Air[N];
int Sol[M];
int count_air = 0;
int count_sol = 0;

// --- PRIMITIVES DE SYNCHRONISATION ---

pthread_mutex_t mutexPiste;

sem_t emptyAir; // Places libres en zone d'approche
sem_t fullAir;  // Avions en attente d'atterrissage
sem_t emptySol; // Places libres en zone de décollage
sem_t fullSol;  // Avions en attente de décollage

// --- FONCTIONS DES THREADS ---

/**
 * @brief Tâche "Producteur" : Place des avions en zone d'approche.
 */
void *ApprocheAvion(void *arg)
{
    int avion_id = 100; // ID unique pour les avions qui approchent
    for (int i = 0; i < 2; i++)
    {
        // Simule l'arrivée d'un nouvel avion
        sleep(rand() % 3 + 1);
        avion_id++;

        P(emptyAir);

        // Section critique pour le buffer Air
        Air[count_air] = avion_id;
        count_air++;
        printf("-> Approche: Avion %d entre en zone d'approche. (Zone Air: %d/%d)\n", avion_id, count_air, N);

        V(fullAir);
    }
    return NULL;
}

/**
 * @brief Tâche "Producteur" : Sort des avions du hangar vers la zone de décollage.
 */
void *SortirAvion(void *arg)
{
    int avion_id = 0; // ID unique pour les avions au sol
    for (int i = 0; i < 2; i++)
    {
        // Simule la préparation d'un avion
        sleep(rand() % 4 + 2);
        avion_id++;

        P(emptySol);

        // Section critique pour le buffer Sol
        Sol[count_sol] = avion_id;
        count_sol++;
        printf("<- Hangar: Avion %d placé en zone de décollage. (Zone Sol: %d/%d)\n", avion_id, count_sol, M);

        V(fullSol);
    }
    return NULL;
}

/**
 * @brief Tâche "Consommateur" : Fait atterrir un avion.
 */
void *Atterrissage(void *arg)
{
    for (int i = 0; i < 2; i++)
    {
        P(fullAir);

        // Demande l'accès à la piste
        pthread_mutex_lock(&mutexPiste);

        // Section critique : utilise la piste
        int avion_atterri = Air[0];
        printf("ATTERRISSAGE: L'avion %d utilise la piste et atterrit.\n", avion_atterri);

        // Met à jour le buffer Air
        for (int i = 0; i < count_air - 1; i++)
        {
            Air[i] = Air[i + 1];
        }
        count_air--;
        sleep(2); // Simule le temps d'occupation de la piste
        printf("ATTERRISSAGE: Piste libérée par l'avion %d. (Zone Air: %d/%d)\n", avion_atterri, count_air, N);

        // Libère la piste
        pthread_mutex_unlock(&mutexPiste);

        V(emptyAir);
    }
    return NULL;
}

/**
 * @brief Tâche "Consommateur" : Fait décoller un avion.
 */
void *Decollage(void *arg)
{
    for (int i = 0; i < 2; i++)
    {
        P(fullSol);

        // Demande l'accès à la piste
        pthread_mutex_lock(&mutexPiste);

        // Section critique : utilise la piste
        int avion_decolle = Sol[0];
        printf("DECOLLAGE: L'avion %d utilise la piste et décolle.\n", avion_decolle);

        // Met à jour le buffer Sol
        for (int i = 0; i < count_sol - 1; i++)
        {
            Sol[i] = Sol[i + 1];
        }
        count_sol--;
        sleep(2); // Simule le temps d'occupation de la piste
        printf("DECOLLAGE: Piste libérée par l'avion %d. (Zone Sol: %d/%d)\n", avion_decolle, count_sol, M);

        // Libère la piste
        pthread_mutex_unlock(&mutexPiste);

        V(emptySol);
    }
    return NULL;
}

int main()
{
    pthread_t th_sortir, th_decollage, th_approche, th_atterrissage;

    // --- INITIALISATION ---
    // Initialiser le mutex pour la piste
    pthread_mutex_init(&mutexPiste, NULL);

    // Initialiser les sémaphores. Le deuxième argument '0' indique
    // qu'ils sont partagés entre les threads d'un même processus.
    sem_init(&emptyAir, 0, N);
    sem_init(&fullAir, 0, 0);
    sem_init(&emptySol, 0, M);
    sem_init(&fullSol, 0, 0);

    printf("Simulation de l'aérodrome démarrée. (Zone Air: %d places, Zone Sol: %d places)\n\n", N, M);

    // --- CRÉATION DES THREADS ---
    pthread_create(&th_approche, NULL, ApprocheAvion, NULL);
    pthread_create(&th_atterrissage, NULL, Atterrissage, NULL);
    pthread_create(&th_sortir, NULL, SortirAvion, NULL);
    pthread_create(&th_decollage, NULL, Decollage, NULL);

    // --- ATTENTE DE LA FIN DES THREADS (ici, boucle infinie) ---
    pthread_join(th_approche, NULL);
    pthread_join(th_atterrissage, NULL);
    pthread_join(th_sortir, NULL);
    pthread_join(th_decollage, NULL);

    // --- DESTRUCTION (never reached because while(1) loops) ---
    pthread_mutex_destroy(&mutexPiste);
    sem_destroy(&emptyAir);
    sem_destroy(&fullAir);
    sem_destroy(&emptySol);
    sem_destroy(&fullSol);

    return 0;
}