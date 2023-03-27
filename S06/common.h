#ifndef __COMMON_H__
#define __COMMON_H__

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ON 1.0
#define OFF 0.0
#define OR(X, Y) (X == ON || Y == ON) ? ON : OFF

#define UP ON
#define RIGHT ON
#define DOWN OFF
#define LEFT OFF

int get_args(int argc, char **argv, int *n, int *m, int *iter_count);
double *init(int n, int m);
double *init_numbered(int n, int m);
void print_ary(double *ary, int n, int m);
void solve_for(double *u, int n, int m, int iter_count);

#endif