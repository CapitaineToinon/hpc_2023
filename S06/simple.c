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

    solve_for(u, n, m, iter_count);
    print_ary(u, n, m);

    free(u);
    return 0;
}