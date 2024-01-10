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
#include "gesummv.h"
static
void init_array(int n,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE A[N][N],
		DATA_TYPE B[N][N],
		DATA_TYPE x[N])
{
  int i, j;
  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < n; i++)
    {
      x[i] = (DATA_TYPE)( i % n) / n;
      for (j = 0; j < n; j++) {
	A[i][j] = (DATA_TYPE) ((i*j+1) % n) / n;
	B[i][j] = (DATA_TYPE) ((i*j+2) % n) / n;
      }
    }
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
void kernel_gesummv(int n,
		    DATA_TYPE alpha,
		    DATA_TYPE beta,
		    DATA_TYPE A[N][N],
		    DATA_TYPE B[N][N],
		    DATA_TYPE tmp[N],
		    DATA_TYPE x[N],
		    DATA_TYPE y[N])
{
  int i, j;
#pragma scop
  for (i = 0; i < _PB_N; i++)
    {
      tmp[i] = SCALAR_VAL(0.0);
      y[i] = SCALAR_VAL(0.0);
      for (j = 0; j < _PB_N; j++)
	{
	  tmp[i] = A[i][j] * x[j] + tmp[i];
	  y[i] = B[i][j] * x[j] + y[i];
	}
      y[i] = alpha * tmp[i] + beta * y[i];
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  DATA_TYPE alpha;
  DATA_TYPE beta;
  DATA_TYPE A[N][N];
  DATA_TYPE B[N][N];
  DATA_TYPE tmp[N];
  DATA_TYPE x[N];
  DATA_TYPE y[N];
  kernel_gesummv (n, alpha, beta,
		  A,
		  B,
		  tmp,
		  x,
		  y);
  return 0;
}
