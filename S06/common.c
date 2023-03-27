#include "common.h"

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

double *init(int n, int m)
{
    double *u = (double *)malloc(n * m * sizeof(double));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            double *p = u + (i * m + j);

            if (i == 0 && j == 0)
            {
                *p = OR(UP, LEFT);
                continue;
            }

            if (i == 0 && j == m - 1)
            {
                *p = OR(UP, RIGHT);
                continue;
            }

            if (i == n - 1 && j == 0)
            {
                *p = OR(DOWN, LEFT);
                continue;
            }

            if (i == n - 1 && j == m - 1)
            {
                *p = OR(DOWN, RIGHT);
                continue;
            }

            if (i == 0)
            {
                *p = UP;
                continue;
            }

            if (i == n - 1)
            {
                *p = DOWN;
                continue;
            }

            if (j == 0)
            {
                *p = LEFT;
                continue;
            }

            if (j == m - 1)
            {
                *p = RIGHT;
                continue;
            }

            *p = OFF;
        }
    }

    return u;
}

void print_ary(double *ary, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%.2f ", ary[i * m + j]);
        }

        printf("\n");
    }
}