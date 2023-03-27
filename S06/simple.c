#include "common.h"

int main(int argc, char **argv)
{
    int n;
    int m;
    int iter_count;

    if (get_args(argc, argv, &n, &m, &iter_count) != 0)
    {
        return EXIT_FAILURE;
    }

    printf("n: %d, m: %d, iter: %d\n", n, m, iter_count);

    double *u = init(n, m);
    double *u_next = init(n, m);

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

    print_ary(u, n, m);

    free(u);
    free(u_next);

    return 0;
}