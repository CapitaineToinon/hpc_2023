#include "common.h"

/// @brief Create a 2D array of doubles
/// @param n amount of lines
/// @param m amount of columns
/// @return pointer to the array
double *create(int n, int m)
{
    return (double *)malloc(n * m * sizeof(double));
}

/// @brief Parses the arguments required for the program
/// @param argc
/// @param argv
/// @param n pointer to the n variable, amount of lines
/// @param m pointer to the m variable, amount of columns
/// @param iter_count pointer to the iter_count variable, amount of iterations
/// @return 0 if success, 1 if failure
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

    return EXIT_SUCCESS;
}

/// @brief Creates a 2D array of doubles and initializes it with the correct starting values values
/// @param n amount of lines
/// @param m amount of columns
/// @return pointer to the array
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

/// @brief Creates a 2D array of doubles and initializes each value with its index
/// @param n amount of lines
/// @param m amount of columns
/// @return pointer to the array
double *init_numbered(int n, int m)
{
    double *u = create(n, m);

    for (int i = 0; i < n * m; i++)
    {
        u[i] = i;
    }

    return u;
}

/// @brief Solves the heat equation into the u_next array given the u array
/// @param u pointer to the current state
/// @param u_next pointer to the next state
/// @param n amount of lines
/// @param m amount of columns
void solve(double *u, double *u_next, int n, int m)
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
}

/// @brief Loops around the u_next array in the same way that solve does but fills it with the probe_value instead
/// @param u pointer to the current state
/// @param u_next pointer to the next state
/// @param n amount of lines
/// @param m amount of columns
/// @param probe_value the probe value to fill the array with
void fake_solve(double *u, double *u_next, int n, int m, int probe_value)
{
    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < m - 1; j++)
        {
            u_next[i * m + j] = probe_value;
        }
    }
}

/// @brief Swaps the pointers of two arrays
/// @param a pointer to the first array
/// @param b pointer to the second array
void swap(double **a, double **b)
{
    double *tmp = *a;
    *a = *b;
    *b = tmp;
}
