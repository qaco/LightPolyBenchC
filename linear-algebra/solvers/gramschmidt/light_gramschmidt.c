/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <polybench.h>
#include "gramschmidt.h"
static
void init_array(int m, int n,
		DATA_TYPE A[M][N],
		DATA_TYPE R[N][N],
		DATA_TYPE Q[M][N])
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {
      A[i][j] = (((DATA_TYPE) ((i*j) % m) / m )*100) + 10;
      Q[i][j] = 0.0;
    }
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      R[i][j] = 0.0;
}
static
void print_array(int m, int n,
		 DATA_TYPE A[M][N],
		 DATA_TYPE R[N][N],
		 DATA_TYPE Q[M][N])
{
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {


    }
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {


    }
}
static
void kernel_gramschmidt(int m, int n,
			DATA_TYPE A[M][N],
			DATA_TYPE R[N][N],
			DATA_TYPE Q[M][N])
{
  int i, j, k;
  DATA_TYPE nrm;
#pragma scop
  for (k = 0; k < _PB_N; k++)
    {
      nrm = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_M; i++)
        nrm += A[i][k] * A[i][k];
      R[k][k] = SQRT_FUN(nrm);
      for (i = 0; i < _PB_M; i++)
        Q[i][k] = A[i][k] / R[k][k];
      for (j = k + 1; j < _PB_N; j++)
	{
	  R[k][j] = SCALAR_VAL(0.0);
	  for (i = 0; i < _PB_M; i++)
	    R[k][j] += Q[i][k] * A[i][j];
	  for (i = 0; i < _PB_M; i++)
	    A[i][j] = A[i][j] - Q[i][k] * R[k][j];
	}
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  int m = M;
  int n = N;
  DATA_TYPE A[M][N];
  DATA_TYPE R[N][N];
  DATA_TYPE Q[M][N];
  kernel_gramschmidt (m, n,
		      A,
		      R,
		      Q);
  return 0;
}
