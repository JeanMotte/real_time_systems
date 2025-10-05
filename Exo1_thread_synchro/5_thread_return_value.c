#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void *roll_dice()
{
    int value = (rand() % 6) + 1;
    int *result = malloc(sizeof(int)); // allocate memory to store the result
    *result = value;                   // store the result in the allocated memory
    // printf("You rolled a %d\n", value);
    // printf("thread %p\n", result);
    return (void *)result;
}

int main()
{
    int *result;
    srand(time(NULL));
    pthread_t th[8];

    for (int i = 0; i < 8; i++)
    {

        if (pthread_create(&th[i], NULL, &roll_dice, NULL) != 0)
        {
            return 1;
        }
    }

    for (int i = 0; i < 8; i++)
    {

        if (pthread_join(th[i], (void **)&result) != 0)
        {
            return 2;
        };
        printf("launch %d: You rolled a %d\n", i + 1, *result);
    }

    // printf("main result: %p\n", result);
    free(result); // free the allocated memory
    return 0;
}