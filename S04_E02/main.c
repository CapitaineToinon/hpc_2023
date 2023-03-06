#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void without_reduce(int rank, int size)
{
    int grades_per_p = 10;
    int grades_total = grades_per_p * size;

    int *grades;
    int *my_grades;

    if (rank == 0)
    {
        grades = malloc(grades_total * sizeof(int));

        for (int i = 0; i < grades_total; i++)
        {
            grades[i] = rand() % 6 + 1;
        }
    }

    my_grades = malloc(grades_per_p * sizeof(int));
    MPI_Scatter(grades, grades_per_p, MPI_INT, my_grades, grades_per_p, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < grades_per_p; i++)
    {
        sum += my_grades[i];
    }

    int *sums = malloc(size * sizeof(int));
    MPI_Gather(&sum, 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int total_sum = 0;

        for (int i = 0; i < size; i++)
        {
            total_sum += sums[i];
        }

        printf("Average: %f\n", (float)total_sum / grades_total);
    }

    free(grades);
    free(my_grades);
    free(sums);
}

void with_reduce(int rank, int size)
{
    int grades_per_p = 10;
    int grades_total = grades_per_p * size;

    int *grades;
    int *sums;

    if (rank == 0)
    {
        grades = malloc(grades_total * sizeof(int));

        for (int i = 0; i < grades_total; i++)
        {
            grades[i] = rand() % 6 + 1;
        }
    }

    sums = malloc(size * sizeof(int));

    MPI_Reduce(grades, sums, grades_total, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int total_sum = 0;

        for (int i = 0; i < size; i++)
        {
            total_sum += sums[i];
        }

        printf("Average: %f\n", (float)total_sum / grades_total);
    }

    free(grades);
    free(sums);
}

int main(int argc, char **argv)
{
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(time(NULL));

    without_reduce(rank, size);
    // with_reduce();

    MPI_Finalize();
}