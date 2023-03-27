#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int get_args(int argc, char **argv, int *n, int *m, int *iter_count)
{
    if (argc < 4)
    {
        printf("Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    *n = atoi(argv[1]);
    *m = atoi(argv[2]);
    *iter_count = atoi(argv[3]);

    return 0;
}

int main(int argc, char **argv)
{
    int n;
    int m;
    int iter_count;

    if (get_args(argc, argv, &n, &m, &iter_count) != 0)
    {
        return EXIT_FAILURE;
    }

    return 0;
}