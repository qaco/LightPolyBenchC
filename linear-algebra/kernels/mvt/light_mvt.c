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
#include "mvt.h"
static
void init_array(int n,
		DATA_TYPE x1[N],
		DATA_TYPE x2[N],
		DATA_TYPE y_1[N],
		DATA_TYPE y_2[N],
		DATA_TYPE A[N][N])
{
  int i, j;
  for (i = 0; i < n; i++)
    {
      x1[i] = (DATA_TYPE) (i % n) / n;
      x2[i] = (DATA_TYPE) ((i + 1) % n) / n;
      y_1[i] = (DATA_TYPE) ((i + 3) % n) / n;
      y_2[i] = (DATA_TYPE) ((i + 4) % n) / n;
      for (j = 0; j < n; j++)
	A[i][j] = (DATA_TYPE) (i*j % n) / n;
    }
}
static
void print_array(int n,
		 DATA_TYPE x1[N],
		 DATA_TYPE x2[N])
{
  int i;
  for (i = 0; i < n; i++) {


  }
  for (i = 0; i < n; i++) {


  }
}
static
void kernel_mvt(int n,
		DATA_TYPE x1[N],
		DATA_TYPE x2[N],
		DATA_TYPE y_1[N],
		DATA_TYPE y_2[N],
		DATA_TYPE A[N][N])
{
  int i, j;
#pragma scop
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      x1[i] = x1[i] + A[i][j] * y_1[j];
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      x2[i] = x2[i] + A[j][i] * y_2[j];
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  volatile DATA_TYPE A[N][N];
  volatile DATA_TYPE x1[N];
  volatile DATA_TYPE x2[N];
  volatile DATA_TYPE y_1[N];
  volatile DATA_TYPE y_2[N];
  polybench_start_instruments;
  kernel_mvt (n,
	      x1,
	      x2,
	      y_1,
	      y_2,
	      A);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(n, x1, x2));
  return 0;
}
