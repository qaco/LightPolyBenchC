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
#include "cholesky.h"
static
void init_array(int n,
		DATA_TYPE A[N][N])
{
  int i, j;
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
		 DATA_TYPE A[N][N])
{
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j <= i; j++) {


  }
}
static
void kernel_cholesky(int n,
		     DATA_TYPE A[N][N])
{
  int i, j, k;
#pragma scop
  for (i = 0; i < _PB_N; i++) {
     //j<i
     for (j = 0; j < i; j++) {
        for (k = 0; k < j; k++) {
           A[i][j] -= A[i][k] * A[j][k];
        }
        A[i][j] /= A[j][j];
     }
     // i==j case
     for (k = 0; k < i; k++) {
        A[i][i] -= A[i][k] * A[i][k];
     }
     A[i][i] = SQRT_FUN(A[i][i]);
  }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  volatile DATA_TYPE A[N][N];
  polybench_start_instruments;
  kernel_cholesky (n, A);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(n, A));
  return 0;
}
