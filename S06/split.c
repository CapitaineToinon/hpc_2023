#include <stdio.h>
#include <stdlib.h>

void split_array(int *arr, int n, int x, int overlap)
{
    int chunk_size = (n + overlap * (x - 1)) / x;
    int extra = (n + overlap * (x - 1)) % x;
    int pos = 0;

    for (int i = 0; i < x; i++)
    {
        int current_chunk_size = (i < extra) ? chunk_size + 1 : chunk_size;
        printf("Chunk %d: ", i);
        for (int j = 0; j < current_chunk_size; j++)
        {
            printf("%d ", arr[pos]);
            pos++;
        }
        pos -= overlap;
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int x = atoi(argv[2]);

    int *arr = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }

    int overlap = 2;

    split_array(arr, n, x, overlap);
    return 0;
}