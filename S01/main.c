#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

float rand_float()
{
    return (float)rand() / (float)RAND_MAX;
}

int throw_arrows(int capacity)
{
    int result = 0;

    for (int i = 0; i < capacity; i++)
    {
        float x = rand_float();
        float y = rand_float();

        if (sqrt(x * x + y * y) <= 1)
        {
            result++;
        }
    }

    return result;
}

float get_total(int hits, int total)
{
    return 4.0 * (float)hits / (float)total;
}

void *throw_arrows_thread(void *args)
{
    int *capacity = (int *)args;
    int *result = (int *)malloc(sizeof(int));
    *result = throw_arrows(*capacity);
    pthread_exit((void *)result);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <points_per_thread> <threads>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    int points_per_thread = atoi(argv[1]);
    int threads = atoi(argv[2]);

    printf("Trying %d points with %d threads\n", points_per_thread, threads);

    {
        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // code à mesurer
        int hits = throw_arrows(points_per_thread * threads);

        clock_gettime(CLOCK_MONOTONIC, &finish);
        double seconds_elapsed = finish.tv_sec - start.tv_sec;
        seconds_elapsed += (finish.tv_nsec - start.tv_nsec) / 1.0e9;
        printf("Without threads\n");
        printf("Temps d'exécution : %f\n", seconds_elapsed);
        printf("Pi = %f\n", get_total(hits, points_per_thread * threads));
    }

    {
        int hits = 0;
        pthread_t *pids = (pthread_t *)malloc(threads * sizeof(pthread_t *));

        struct timespec start, finish;
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < threads; i++)
        {
            pthread_create(&pids[i], NULL, throw_arrows_thread, &points_per_thread);
        }

        for (int i = 0; i < threads; i++)
        {
            void *res;
            pthread_join(pids[i], &res);
            hits += *(int *)res;
        }

        clock_gettime(CLOCK_MONOTONIC, &finish);
        double seconds_elapsed = finish.tv_sec - start.tv_sec;
        seconds_elapsed += (finish.tv_nsec - start.tv_nsec) / 1.0e9;

        printf("With threads\n");
        printf("Temps d'exécution : %f\n", seconds_elapsed);
        printf("Pi = %f\n", get_total(hits, points_per_thread * threads));

        free(pids);
    }

    return 0;
}