// #include "common.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int get_args(int argc, char **argv, int *n, int *m, int *iter_count)
{
    if (argc < 4)
    {
        printf("Usage: %s <n> <m> <iter>\n", argv[0]);
        return EXIT_FAILURE;
    }

    *n = atoi(argv[1]);
    *m = atoi(argv[2]);
    *iter_count = atoi(argv[3]);

    return 0;
}

void get_sendcounts_displs(double *values, int n, int m, int chunk_count, int overlap_per_side, int *sendcounts, int *displs)
{
    int chunk_size = (n + (overlap_per_side * 2) * (chunk_count - 1)) / chunk_count;
    int extra = (n + (overlap_per_side * 2) * (chunk_count - 1)) % chunk_count;
    int pos = 0;

    if (chunk_size <= (overlap_per_side * 2))
    {
        printf("Chunk size is too small, aborting\n");
        exit(1);
    }

    printf("Chunk size: %d, extra: %d\n", chunk_size, extra);

    for (int i = 0; i < chunk_count; i++)
    {
        int current_chunk_size = (i < extra) ? chunk_size + 1 : chunk_size;
        int from_n = pos;
        int to_n = pos + current_chunk_size;
        int count_n = to_n - from_n;

        printf("Chunk %d gets %d lines: \n", i, count_n);
        for (int j = from_n; j < to_n; j++)
        {
            printf("%d ", j);
        }
        printf("\n");

        sendcounts[i] = count_n * m;
        displs[i] = from_n * m;

        pos += current_chunk_size - (overlap_per_side * 2);
    }
}

int main(int argc, char **argv)
{
    // int myRank;
    // int nProc;

    // MPI_Init(&argc, &argv);
    // MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    // MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int n;
    int m;
    int iter_count;

    if (get_args(argc, argv, &n, &m, &iter_count) != 0)
    {
        return EXIT_FAILURE;
    }

    printf("n = %d, m = %d, iter = %d\n", n, m, iter_count);

    double *u = (double *)malloc(n * m * sizeof(double));

    for (int i = 0; i < n * m; i++)
    {
        u[i] = i;
    }

    int nProc = 3;
    int *sendcounts = (int *)malloc(nProc * sizeof(int));
    int *displs = (int *)malloc(nProc * sizeof(int));

    get_sendcounts_displs(u, n, m, nProc, 1, sendcounts, displs);

    for (int i = 0; i < nProc; i++)
    {
        printf("Chunk %d has %d lines aka %d bytes, displ %d\n", i, sendcounts[i] / m, sendcounts[i], displs[i]);

        for (int j = 0; j < sendcounts[i]; j++)
        {
            if (j % m == 0)
                printf("\n");

            printf("%02.0f ", u[displs[i] + j]);
        }

        printf("\n");
        printf("\n");
    }

    return 0;
}