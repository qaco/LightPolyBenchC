/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* gemver.c: this file is part of PolyBench/C */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
/* Include polybench common header. */
#include <polybench.h>
/* Include benchmark-specific header. */
#include "gemver.h"
/* Array initialization. */
static
void init_array (int n,
		 DATA_TYPE *alpha,
		 DATA_TYPE *beta,
		 DATA_TYPE A[N][N],
		 DATA_TYPE u1[N],
		 DATA_TYPE v1[N],
		 DATA_TYPE u2[N],
		 DATA_TYPE v2[N],
		 DATA_TYPE w[N],
		 DATA_TYPE x[N],
		 DATA_TYPE y[N],
		 DATA_TYPE z[N])
{
  int i, j;
  *alpha = 1.5;
  *beta = 1.2;
  DATA_TYPE fn = (DATA_TYPE)n;
  for (i = 0; i < n; i++)
    {
      u1[i] = i;
      u2[i] = ((i+1)/fn)/2.0;
      v1[i] = ((i+1)/fn)/4.0;
      v2[i] = ((i+1)/fn)/6.0;
      y[i] = ((i+1)/fn)/8.0;
      z[i] = ((i+1)/fn)/9.0;
      x[i] = 0.0;
      w[i] = 0.0;
      for (j = 0; j < n; j++)
        A[i][j] = (DATA_TYPE) (i*j % n) / n;
    }
}
/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
		 DATA_TYPE w[N])
{
  int i;
  for (i = 0; i < n; i++) {


  }
}
/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gemver(int n,
		   DATA_TYPE alpha,
		   DATA_TYPE beta,
		   DATA_TYPE A[N][N],
		   DATA_TYPE u1[N],
		   DATA_TYPE v1[N],
		   DATA_TYPE u2[N],
		   DATA_TYPE v2[N],
		   DATA_TYPE w[N],
		   DATA_TYPE x[N],
		   DATA_TYPE y[N],
		   DATA_TYPE z[N])
{
  int i, j;
#pragma scop
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      A[i][j] = A[i][j] + u1[i] * v1[j] + u2[i] * v2[j];
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      x[i] = x[i] + beta * A[j][i] * y[j];
  for (i = 0; i < _PB_N; i++)
    x[i] = x[i] + z[i];
  for (i = 0; i < _PB_N; i++)
    for (j = 0; j < _PB_N; j++)
      w[i] = w[i] +  alpha * A[i][j] * x[j];
#pragma endscop
}
int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  volatile DATA_TYPE A[N][N];
  volatile DATA_TYPE u1[N];
  volatile DATA_TYPE v1[N];
  volatile DATA_TYPE u2[N];
  volatile DATA_TYPE v2[N];
  volatile DATA_TYPE w[N];
  volatile DATA_TYPE x[N];
  volatile DATA_TYPE y[N];
  volatile DATA_TYPE z[N];
  /* Initialize array(s). */
  /* Start timer. */
  polybench_start_instruments;
  /* Run kernel. */
  kernel_gemver (n, alpha, beta,
		 A,
		 u1,
		 v1,
		 u2,
		 v2,
		 w,
		 x,
		 y,
		 z);
  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;
  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  /* Be clean. */
  return 0;
}
