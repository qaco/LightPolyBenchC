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
#include "symm.h"
static
void init_array(int m, int n,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE C[M][N],
		DATA_TYPE A[M][M],
		DATA_TYPE B[M][N])
{
  int i, j;
  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {
      C[i][j] = (DATA_TYPE) ((i+j) % 100) / m;
      B[i][j] = (DATA_TYPE) ((n+i-j) % 100) / m;
    }
  for (i = 0; i < m; i++) {
    for (j = 0; j <=i; j++)
      A[i][j] = (DATA_TYPE) ((i+j) % 100) / m;
    for (j = i+1; j < m; j++)
      A[i][j] = -999; //regions of arrays that should not be used
  }
}
static
void print_array(int m, int n,
		 DATA_TYPE C[M][N])
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {


    }
}
static
void kernel_symm(int m, int n,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE C[M][N],
		 DATA_TYPE A[M][M],
		 DATA_TYPE B[M][N])
{
  int i, j, k;
  DATA_TYPE temp2;
//BLAS PARAMS
//SIDE = 'L'
//UPLO = 'L'
// =>  Form  C := alpha*A*B + beta*C
// A is MxM
// B is MxN
// C is MxN
//note that due to Fortran array layout, the code below more closely resembles upper triangular case in BLAS
#pragma scop
   for (i = 0; i < _PB_M; i++)
      for (j = 0; j < _PB_N; j++ )
      {
        temp2 = 0;
        for (k = 0; k < i; k++) {
           C[k][j] += alpha*B[i][j] * A[i][k];
           temp2 += B[k][j] * A[i][k];
        }
        C[i][j] = beta * C[i][j] + alpha*B[i][j] * A[i][i] + alpha * temp2;
     }
#pragma endscop
}
int main(int argc, char** argv)
{
  int m = M;
  int n = N;
  DATA_TYPE alpha;
  DATA_TYPE beta;
  volatile DATA_TYPE C[M][N];
  volatile DATA_TYPE A[M][M];
  volatile DATA_TYPE B[M][N];
  polybench_start_instruments;
  kernel_symm (m, n,
	       alpha, beta,
	       C,
	       A,
	       B);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(m, n, C));
  return 0;
}
