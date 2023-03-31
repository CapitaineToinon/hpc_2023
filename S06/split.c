#include "common.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// the amount of overlapping lines between different chunks
// for example an array [1, 2, 3, 4] with overlap 1 shared bewteen two chunks
// would look like this:
// chunk 1: [1, 2, 3]
// chunk 2: [2, 3, 4]
#define OVERLAP_PER_SIDE 1
#define ROOT 0

#ifdef DEBUG
#define INIT(n, m) init_numbered(n, m)
#else
#define INIT(n, m) init(n, m)
#endif

#ifdef DEBUG
#define SOLVE(u, u_next, n, m, probe) fake_solve(u, u_next, n, m, probe)
#else
#define SOLVE(u, u_next, n, m, _probe) solve(u, u_next, n, m)
#endif

void get_sendcounts_displs(int n, int m, int chunk_count, int overlap_per_side, int *sendcounts, int *displs)
{
    int chunk_size = (n + (overlap_per_side * 2) * (chunk_count - 1)) / chunk_count;
    int extra = (n + (overlap_per_side * 2) * (chunk_count - 1)) % chunk_count;
    int pos = 0;

    if (chunk_size <= (overlap_per_side * 2))
    {
        printf("Chunk size is too small, aborting\n");
        exit(1);
    }

#ifdef DEBUG
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == ROOT)
    {
        printf("Chunk size: %d, extra: %d\n", chunk_size, extra);
    }
#endif

    for (int i = 0; i < chunk_count; i++)
    {
        int current_chunk_size = (i < extra) ? chunk_size + 1 : chunk_size;
        int from_n = pos;
        int to_n = pos + current_chunk_size;
        int count_n = to_n - from_n;

#ifdef DEBUG
        int my_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

        if (my_rank == ROOT)
        {
            printf("Chunk %d gets %d lines: \n", i, count_n);
            for (int j = from_n; j < to_n; j++)
            {
                printf("%d ", j);
            }
            printf("\n");
        }
#endif

        sendcounts[i] = count_n * m;
        displs[i] = from_n * m;

        pos += current_chunk_size - (overlap_per_side * 2);
    }
}

int main(int argc, char **argv)
{
    int my_rank;
    int n_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    int n;
    int m;
    int iter_count;

    if (get_args(argc, argv, &n, &m, &iter_count) != 0)
    {
        return EXIT_FAILURE;
    }

#ifdef DEBUG
    if (my_rank == ROOT)
    {
        printf("n = %d, m = %d, iter = %d\n", n, m, iter_count);
    }
#endif

    double *u;

    if (my_rank == ROOT)
    {
        u = INIT(n, m);
    }

    int *sendcounts = (int *)malloc(n_proc * sizeof(int));
    int *displs = (int *)malloc(n_proc * sizeof(int));

    get_sendcounts_displs(n, m, n_proc, OVERLAP_PER_SIDE, sendcounts, displs);

    double *u_local = (double *)malloc(sendcounts[my_rank] * sizeof(double));
    double *u_local_next = (double *)malloc(sendcounts[my_rank] * sizeof(double));

    MPI_Scatterv(u, sendcounts, displs, MPI_DOUBLE, u_local, sendcounts[my_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // ensure the next array starts at the same state as the current one
    memcpy(u_local_next, u_local, sendcounts[my_rank] * sizeof(double));

    for (int iter = 0; iter < iter_count; iter++)
    {
        // solve our current chunk locally
        SOLVE(u_local, u_local_next, sendcounts[my_rank] / m, m, iter + 1);
        swap(&u_local, &u_local_next);

        if (my_rank != n_proc - 1)
        {
            // If not the last rank, send the lowest line processed to the next rank
            // to update its top line
            MPI_Request _ignored;
            MPI_Isend(u_local + (sendcounts[my_rank] - (2 * m)), m, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, &_ignored);

            // If not the last rank, update my bottom line with the top line of the next rank
            MPI_Recv(u_local + (sendcounts[my_rank] - m), m, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (my_rank != 0)
        {
            // If not the first rank, receive the top line from the previous rank
            MPI_Recv(u_local, m, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // If not the first rank, send my top line to the previous rank
            // to update its bottom line
            MPI_Request _ignored;
            MPI_Isend(u_local + m, m, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, &_ignored);
        }
    }

#ifdef DEBUG
    sleep(my_rank);

    printf("Rank %d has %d lines: ", my_rank, sendcounts[my_rank] / m);
    for (int i = 0; i < sendcounts[my_rank]; i++)
    {
        if (i % m == 0)
        {
            printf("\n");
        }

        printf("%05.2f ", u_local[i]);
    }
    printf("\n");
#endif

    // Could be smarter here and not gather the overlapping lines multiple times
    // but this is just easier and probably not even worth the hassle
    MPI_Gatherv(u_local, sendcounts[my_rank], MPI_DOUBLE, u, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == ROOT)
    {

#ifdef DEBUG
        sleep(1);
        printf("Final output:\n");
#endif

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                printf("%05.2f ", u[i * m + j]);
            }
            printf("\n");
        }
    }

    // free everything
    free(u);
    free(sendcounts);
    free(displs);
    free(u_local);
    free(u_local_next);

    MPI_Finalize();

    return 0;
}