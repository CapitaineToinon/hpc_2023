#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int myRank;
    int nProc;
    const int size = 1000000;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    const int voisin_gauche = (myRank + nProc - 1) % nProc;
    const int voisin_droite = (myRank + 1) % nProc;

    int *bufSend = malloc(size * sizeof(int));
    int *bufRecv = malloc(size * sizeof(int));

    bufSend[0] = myRank;

    for (int iEnvoi = 0; iEnvoi < nProc; ++iEnvoi)
    {
        MPI_Request request;
        MPI_Isend(&bufSend[0], size, MPI_INT, voisin_droite, 0,
                  MPI_COMM_WORLD, &request);
        MPI_Recv(&bufRecv[0], size, MPI_INT, voisin_gauche, 0,
                 MPI_COMM_WORLD, &status);

        bufSend[0] = bufRecv[0];
        if (myRank == 0)
        {
            printf("Le message actuel est %d\n", bufRecv[0]);
        }
    }
    MPI_Finalize();
}
