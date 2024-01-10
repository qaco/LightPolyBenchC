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
#include "trmm.h"
static
void init_array(int m, int n,
		DATA_TYPE *alpha,
		DATA_TYPE A[M][M],
		DATA_TYPE B[M][N])
{
  int i, j;
  *alpha = 1.5;
  for (i = 0; i < m; i++) {
    for (j = 0; j < i; j++) {
      A[i][j] = (DATA_TYPE)((i+j) % m)/m;
    }
    A[i][i] = 1.0;
    for (j = 0; j < n; j++) {
      B[i][j] = (DATA_TYPE)((n+(i-j)) % n)/n;
    }
 }
}
static
void print_array(int m, int n,
		 DATA_TYPE B[M][N])
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {


    }
}
static
void kernel_trmm(int m, int n,
		 DATA_TYPE alpha,
		 DATA_TYPE A[M][M],
		 DATA_TYPE B[M][N])
{
  int i, j, k;
//BLAS parameters
//SIDE   = 'L'
//UPLO   = 'L'
//TRANSA = 'T'
//DIAG   = 'U'
// => Form  B := alpha*A**T*B.
// A is MxM
// B is MxN
#pragma scop
  for (i = 0; i < _PB_M; i++)
     for (j = 0; j < _PB_N; j++) {
        for (k = i+1; k < _PB_M; k++)
           B[i][j] += A[k][i] * B[k][j];
        B[i][j] = alpha * B[i][j];
     }
#pragma endscop
}
int main(int argc, char** argv)
{
  int m = M;
  int n = N;
  DATA_TYPE alpha;
  volatile DATA_TYPE A[M][M];
  volatile DATA_TYPE B[M][N];
  kernel_trmm (m, n, alpha, A, B);
  polybench_prevent_dce(print_array(m, n, B));
  return 0;
}
