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
#include "trisolv.h"
static
void init_array(int n,
		DATA_TYPE L[N][N],
		DATA_TYPE x[N],
		DATA_TYPE b[N])
{
  int i, j;
  for (i = 0; i < n; i++)
    {
      x[i] = - 999;
      b[i] =  i ;
      for (j = 0; j <= i; j++)
	L[i][j] = (DATA_TYPE) (i+n-j+1)*2/n;
    }
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
void kernel_trisolv(int n,
		    DATA_TYPE L[N][N],
		    DATA_TYPE x[N],
		    DATA_TYPE b[N])
{
  int i, j;
#pragma scop
  for (i = 0; i < _PB_N; i++)
    {
      x[i] = b[i];
      for (j = 0; j <i; j++)
        x[i] -= L[i][j] * x[j];
      x[i] = x[i] / L[i][i];
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  DATA_TYPE L[N][N];
  DATA_TYPE x[N];
  DATA_TYPE b[N];
  kernel_trisolv (n, L, x, b);
  return 0;
}
