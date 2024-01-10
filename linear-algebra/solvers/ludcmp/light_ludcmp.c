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
#include "ludcmp.h"
static
void init_array (int n,
		 DATA_TYPE A[N][N],
		 DATA_TYPE b[N],
		 DATA_TYPE x[N],
		 DATA_TYPE y[N])
{
  int i, j;
  DATA_TYPE fn = (DATA_TYPE)n;
  for (i = 0; i < n; i++)
    {
      x[i] = 0;
      y[i] = 0;
      b[i] = (i+1)/fn/2.0 + 4;
    }
  for (i = 0; i < n; i++)
    {
      for (j = 0; j <= i; j++)
	A[i][j] = (DATA_TYPE)(-j % n) / n + 1;
      for (j = i+1; j < n; j++) {
	A[i][j] = 0;
      }
      A[i][i] = 1;
    }
  int r,s,t;
  volatile DATA_TYPE B[N][N];
  for (r = 0; r < n; ++r)
    for (s = 0; s < n; ++s)
      (B)[r][s] = 0;
  for (t = 0; t < n; ++t)
    for (r = 0; r < n; ++r)
      for (s = 0; s < n; ++s)
	(B)[r][s] += A[r][t] * A[s][t];
    for (r = 0; r < n; ++r)
      for (s = 0; s < n; ++s)
	A[r][s] = (B)[r][s];
}
static
void print_array(int n,
		 DATA_TYPE x[N])
{
  int i;
  for (i = 0; i < n; i++) {


  }
}
static
void kernel_ludcmp(int n,
		   DATA_TYPE A[N][N],
		   DATA_TYPE b[N],
		   DATA_TYPE x[N],
		   DATA_TYPE y[N])
{
  int i, j, k;
  DATA_TYPE w;
#pragma scop
  for (i = 0; i < _PB_N; i++) {
    for (j = 0; j <i; j++) {
       w = A[i][j];
       for (k = 0; k < j; k++) {
          w -= A[i][k] * A[k][j];
       }
        A[i][j] = w / A[j][j];
    }
   for (j = i; j < _PB_N; j++) {
       w = A[i][j];
       for (k = 0; k < i; k++) {
          w -= A[i][k] * A[k][j];
       }
       A[i][j] = w;
    }
  }
  for (i = 0; i < _PB_N; i++) {
     w = b[i];
     for (j = 0; j < i; j++)
        w -= A[i][j] * y[j];
     y[i] = w;
  }
   for (i = _PB_N-1; i >=0; i--) {
     w = y[i];
     for (j = i+1; j < _PB_N; j++)
        w -= A[i][j] * x[j];
     x[i] = w / A[i][i];
  }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  volatile DATA_TYPE A[N][N];
  volatile DATA_TYPE b[N];
  volatile DATA_TYPE x[N];
  volatile DATA_TYPE y[N];
  polybench_start_instruments;
  kernel_ludcmp (n,
		 A,
		 b,
		 x,
		 y);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(n, x));
  return 0;
}
