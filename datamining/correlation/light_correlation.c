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
#include "correlation.h"
static
void init_array (int m,
		 int n,
		 DATA_TYPE *float_n,
		 DATA_TYPE data[N][M])
{
  int i, j;
  *float_n = (DATA_TYPE)N;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
      data[i][j] = (DATA_TYPE)(i*j)/M + i;
}
static
void print_array(int m,
		 DATA_TYPE corr[M][M])
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) {


    }
}
static
void kernel_correlation(int m, int n,
			DATA_TYPE float_n,
			DATA_TYPE data[N][M],
			DATA_TYPE corr[M][M],
			DATA_TYPE mean[M],
			DATA_TYPE stddev[M])
{
  int i, j, k;
  DATA_TYPE eps = SCALAR_VAL(0.1);
#pragma scop
  for (j = 0; j < _PB_M; j++)
    {
      mean[j] = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_N; i++)
	mean[j] += data[i][j];
      mean[j] /= float_n;
    }
   for (j = 0; j < _PB_M; j++)
    {
      stddev[j] = SCALAR_VAL(0.0);
      for (i = 0; i < _PB_N; i++)
        stddev[j] += (data[i][j] - mean[j]) * (data[i][j] - mean[j]);
      stddev[j] /= float_n;
      stddev[j] = SQRT_FUN(stddev[j]);
      stddev[j] = stddev[j] <= eps ? SCALAR_VAL(1.0) : stddev[j];
    }
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_M; j++)
      {
        data[i][j] -= mean[j];
        data[i][j] /= SQRT_FUN(float_n) * stddev[j];
      }
  /* Calculate the m * m correlation matrix. */
  for (i = 0; i < _PB_M-1; i++)
    {
      corr[i][i] = SCALAR_VAL(1.0);
      for (j = i+1; j < _PB_M; j++)
        {
          corr[i][j] = SCALAR_VAL(0.0);
          for (k = 0; k < _PB_N; k++)
            corr[i][j] += (data[k][i] * data[k][j]);
          corr[j][i] = corr[i][j];
        }
    }
  corr[_PB_M-1][_PB_M-1] = SCALAR_VAL(1.0);
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  int m = M;
  DATA_TYPE float_n;
  volatile DATA_TYPE data[N][M];
  volatile DATA_TYPE corr[M][M];
  volatile DATA_TYPE mean[M];
  volatile DATA_TYPE stddev[M];
  kernel_correlation (m, n, float_n,
		      data,
		      corr,
		      mean,
		      stddev);
  polybench_prevent_dce(print_array(m, corr));
  return 0;
}
