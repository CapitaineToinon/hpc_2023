#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
    int myRank;
    int nProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    bool ping = true;
    int other = 1;

    int k = 10;

    if (myRank == other)
    {
        ping = false;
        other = 0;
    }

    while (k--)
    {
        if (myRank % 2 == 0)
        {
            int BALL = 42;
            printf("Sending %d\n", BALL);
            MPI_Send(&BALL, 1, MPI_INT, other, 0, MPI_COMM_WORLD);
        }
        else
        {
            int BALL = 0;
            MPI_Recv(&BALL, 1, MPI_INT, other, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Received %d\n", BALL);
        }

        ping = !ping;
    }

    MPI_Finalize();
}
