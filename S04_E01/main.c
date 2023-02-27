#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int HEPIA_Bcast(void *data, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int myRank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (myRank == root)
    {
        for (int i = 0; i < size; i++)
        {
            if (i != root)
            {
                int status = MPI_Send(data, count, datatype, i, 0, comm);

                if (status != MPI_SUCCESS)
                {
                    return status;
                }
            }
        }

        return MPI_SUCCESS;
    }
    else
    {
        return MPI_Recv(data, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char **argv)
{
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int BALL = 0;

    if (rank == 0)
    {
        // olala this is a very expensive computation
        BALL = 42;
    }

    int status = HEPIA_Bcast(&BALL, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("[%d] ball is %d with status %d\n", rank, BALL, status);

    MPI_Finalize();
}
