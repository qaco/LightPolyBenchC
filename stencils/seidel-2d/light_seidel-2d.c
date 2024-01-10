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
#include "seidel-2d.h"
static
void init_array (int n,
		 DATA_TYPE A[N][N])
{
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      A[i][j] = ((DATA_TYPE) i*(j+2) + 2) / n;
}
static
void print_array(int n,
		 DATA_TYPE A[N][N])
{
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {


    }
}
static
void kernel_seidel_2d(int tsteps,
		      int n,
		      DATA_TYPE A[N][N])
{
  int t, i, j;
#pragma scop
  for (t = 0; t <= _PB_TSTEPS - 1; t++)
    for (i = 1; i<= _PB_N - 2; i++)
      for (j = 1; j <= _PB_N - 2; j++)
	A[i][j] = (A[i-1][j-1] + A[i-1][j] + A[i-1][j+1]
		   + A[i][j-1] + A[i][j] + A[i][j+1]
		   + A[i+1][j-1] + A[i+1][j] + A[i+1][j+1])/SCALAR_VAL(9.0);
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  int tsteps = TSTEPS;
  volatile DATA_TYPE A[N][N];
  polybench_start_instruments;
  kernel_seidel_2d (tsteps, n, A);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(n, A));
  return 0;
}
