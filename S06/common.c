#include "common.h"

double *create(int n, int m)
{
    return (double *)malloc(n * m * sizeof(double));
}

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
    double *u = create(n, m);

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

double *init_numbered(int n, int m)
{
    double *u = create(n, m);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            u[i * m + j] = i * m + j;
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
            printf("%05.2f ", ary[i * m + j]);
        }

        printf("\n");
    }
}

void solve_for(double *u, int n, int m, int iter_count)
{
    double *u_next = create(n, m);
    memcpy(u_next, u, n * m * sizeof(double));

    for (int iter = 0; iter < iter_count; iter++)
    {
        for (int i = 1; i < n - 1; i++)
        {
            for (int j = 1; j < m - 1; j++)
            {
                u_next[i * m + j] = (u[(i - 1) * m + j] +
                                     u[(i + 1) * m + j] +
                                     u[i * m + j - 1] +
                                     u[i * m + j + 1]) /
                                    4;
            }
        }

        memcpy(u, u_next, n * m * sizeof(double));
    }

    free(u_next);
}
