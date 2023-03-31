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

    for (int i = 0; i < iter_count; i++)
    {
        solve(u, u_next, n, m);
        swap(&u, &u_next);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%05.2f ", u[i * m + j]);
        }

        printf("\n");
    }

    free(u);
    free(u_next);

    return 0;
}