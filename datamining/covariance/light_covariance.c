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
#include "covariance.h"
static
void init_array (int m, int n,
		 DATA_TYPE *float_n,
		 DATA_TYPE data[N][M])
{
  int i, j;
  *float_n = (DATA_TYPE)n;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
      data[i][j] = ((DATA_TYPE) i*j) / M;
}
static
void print_array(int m,
		 DATA_TYPE cov[M][M])
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) {


    }
}
static
void kernel_covariance(int m, int n,
		       DATA_TYPE float_n,
		       DATA_TYPE data[N][M],
		       DATA_TYPE cov[M][M],
		       DATA_TYPE mean[M])
{
  int i, j, k;
#pragma scop
  for (j = 0; j < _PB_M; j++)
    {
      mean[j] = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_N; i++)
        mean[j] += data[i][j];
      mean[j] /= float_n;
    }
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_M; j++)
      data[i][j] -= mean[j];
  for (i = 0; i < _PB_M; i++)
    for (j = i; j < _PB_M; j++)
      {
        cov[i][j] = SCALAR_VAL(0.0);
        for (k = 0; k < _PB_N; k++)
	  cov[i][j] += data[k][i] * data[k][j];
        cov[i][j] /= (float_n - SCALAR_VAL(1.0));
        cov[j][i] = cov[i][j];
      }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  int m = M;
  DATA_TYPE float_n;
  volatile DATA_TYPE data[N][M];
  volatile DATA_TYPE cov[M][M];
  volatile DATA_TYPE mean[M];
  polybench_start_instruments;
  kernel_covariance (m, n, float_n,
		     data,
		     cov,
		     mean);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(m, cov));
  return 0;
}
