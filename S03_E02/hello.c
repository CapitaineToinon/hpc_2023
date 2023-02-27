#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int myRank;
  int nProc;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProc);

  printf("Hello, I'm %d\n", myRank);

  MPI_Finalize();
}
