#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int const MAX_ARY_SIZE = 10;

void print_ary(int *ary, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", ary[i]);
    }

    printf("\n");
}

void initRndGen(int rank)
{
    srand(time(NULL) + rank);
}

int getInt(int max)
{
    return rand() % max + 1;
}

int *generateAry(int n)
{
    int *ary = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        ary[i] = i + 1;
    }
    return ary;
}

// temps de sommeil aléatoire entre 1 et 10 secondes
void napTime()
{
    int sleepTime = rand() % 10 + 1;
    sleep(sleepTime);
}

int main(int argc, char **argv)
{
    int myRank;
    int nProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    initRndGen(myRank);

    if (myRank == 0)
    {
        MPI_Status status;

        int **arrays = malloc(nProc * sizeof(int *));

        for (int i = 1; i < nProc; i++)
        {
            printf("Rank %d: waiting for message...\n", myRank);

            int n = 0;
            MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Rank %d: %d will send %d ints, allocating...\n", myRank, status.MPI_SOURCE, n);

            arrays[status.MPI_SOURCE] = malloc(n * sizeof(int));
            MPI_Recv(arrays[status.MPI_SOURCE], n, MPI_INT, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            printf("Rank %d: %d sent %d ints: ", myRank, status.MPI_SOURCE, n);
            print_ary(arrays[status.MPI_SOURCE], n);
        }

        free(arrays);
    }
    else
    {
        // Création de la donnée
        int n = getInt(MAX_ARY_SIZE);
        int *arr = generateAry(n);

        printf("Rank %d: sending %d ints\n", myRank, n);

        napTime();
        MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        napTime();
        MPI_Send(arr, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
