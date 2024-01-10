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
#include "atax.h"
static
void init_array (int m, int n,
		 DATA_TYPE A[M][N],
		 DATA_TYPE x[N])
{
  int i, j;
  DATA_TYPE fn;
  fn = (DATA_TYPE)n;
  for (i = 0; i < n; i++)
      x[i] = 1 + (i / fn);
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      A[i][j] = (DATA_TYPE) ((i+j) % n) / (5*m);
}
static
void print_array(int n,
		 DATA_TYPE y[N])
{
  int i;
  for (i = 0; i < n; i++) {


  }
}
static
void kernel_atax(int m, int n,
		 DATA_TYPE A[M][N],
		 DATA_TYPE x[N],
		 DATA_TYPE y[N],
		 DATA_TYPE tmp[M])
{
  int i, j;
#pragma scop
  for (i = 0; i < _PB_N; i++)
    y[i] = 0;
  for (i = 0; i < _PB_M; i++)
    {
      tmp[i] = SCALAR_VAL(0.0);
      for (j = 0; j < _PB_N; j++)
	tmp[i] = tmp[i] + A[i][j] * x[j];
      for (j = 0; j < _PB_N; j++)
	y[j] = y[j] + A[i][j] * tmp[i];
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  int m = M;
  int n = N;
  volatile DATA_TYPE A[M][N];
  volatile DATA_TYPE x[N];
  volatile DATA_TYPE y[N];
  volatile DATA_TYPE tmp[M];
  polybench_start_instruments;
  kernel_atax (m, n,
	       A,
	       x,
	       y,
	       tmp);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(n, y));
  return 0;
}
