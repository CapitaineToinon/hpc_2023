#include "common.h"
#include <unistd.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

double *solve(int n, int m, int iter_count)
{
    // double *u = init_numbered(n, m);
    double *u = init(n, m);

    // if (n < 3 || m < 3)
    // {
    //     return u;
    // }

    // double *u_next = init(n, m);

    int myRank;
    int nProc;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    printf("myRank: %d, nProc: %d\n", myRank, nProc);

    int *sendcounts = malloc(nProc * sizeof(int));
    int *senddispls = malloc(nProc * sizeof(int));
    int *recvcounts = malloc(nProc * sizeof(int));
    int *recvdispls = malloc(nProc * sizeof(int));

    for (int i = 0; i < nProc; i++)
    {
        sendcounts[i] = ((n / nProc) + 1) * m;
        senddispls[i] = ((n / nProc) * m * i) - (m * MIN(i, 1));
        recvcounts[i] = ((n / nProc) * m) - m;
        recvdispls[i] = ((n / nProc) * m * i) + m;
    }

    if (myRank == 0)
    {
        printf("sendcounts: ");
        for (int i = 0; i < nProc; i++)
        {
            printf("%d ", sendcounts[i]);
        }
        printf("\n");

        printf("senddispls: ");
        for (int i = 0; i < nProc; i++)
        {
            printf("%d ", senddispls[i]);
        }
        printf("\n");

        printf("recvcounts: ");
        for (int i = 0; i < nProc; i++)
        {
            printf("%d ", recvcounts[i]);
        }
        printf("\n");

        printf("recvdispls: ");
        for (int i = 0; i < nProc; i++)
        {
            printf("%d ", recvdispls[i]);
        }
        printf("\n");
    }

    /**
     * Works for 1 iteration but not for more
     * problem is that the outer values are not updated and so the inner get wrong
     *
     * Maybe I should have not one but two padded lines?
     */
    MPI_Scatterv(u, sendcounts, senddispls, MPI_DOUBLE, u, sendcounts[myRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (myRank == 1)
    {
        sleep(1);
    }

    // solve locally
    solve_for(u, sendcounts[myRank] / m, m, iter_count);

    printf("myRank: %d, u: \n", myRank);
    print_ary(u, sendcounts[myRank] / m, m);

    MPI_Gatherv(u + m, recvcounts[myRank], MPI_DOUBLE, u + m, recvcounts, senddispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return u;
}

int main(int argc, char **argv)
{
    int myRank;
    int nProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    int n;
    int m;
    int iter_count;

    if (get_args(argc, argv, &n, &m, &iter_count) != 0)
    {
        return EXIT_FAILURE;
    }

    double *u = solve(n, m, iter_count);

    if (myRank == 0)
    {
        sleep(2);
        double *s = init(n, m);
        solve_for(s, n, m, iter_count);
        printf("Expected:\n");
        print_ary(s, n, m);
        sleep(1);
        printf("actual:\n");
        print_ary(u, n, m);
    }

    free(u);

    MPI_Finalize();

    return 0;
}
