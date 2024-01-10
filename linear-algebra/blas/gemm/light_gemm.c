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
#include "gemm.h"
static
void init_array(int ni, int nj, int nk,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE C[NI][NJ],
		DATA_TYPE A[NI][NK],
		DATA_TYPE B[NK][NJ])
{
  int i, j;
  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++)
      C[i][j] = (DATA_TYPE) ((i*j+1) % ni) / ni;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE) (i*(j+1) % nk) / nk;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE) (i*(j+2) % nj) / nj;
}
static
void print_array(int ni, int nj,
		 DATA_TYPE C[NI][NJ])
{
  int i, j;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {


    }
}
static
void kernel_gemm(int ni, int nj, int nk,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE C[NI][NJ],
		 DATA_TYPE A[NI][NK],
		 DATA_TYPE B[NK][NJ])
{
  int i, j, k;
//BLAS PARAMS
//TRANSA = 'N'
//TRANSB = 'N'
// => Form C := alpha*A*B + beta*C,
//A is NIxNK
//B is NKxNJ
//C is NIxNJ
#pragma scop
  for (i = 0; i < _PB_NI; i++) {
    for (j = 0; j < _PB_NJ; j++)
	C[i][j] *= beta;
    for (k = 0; k < _PB_NK; k++) {
       for (j = 0; j < _PB_NJ; j++)
	  C[i][j] += alpha * A[i][k] * B[k][j];
    }
  }
#pragma endscop
}
int main(int argc, char** argv)
{
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  DATA_TYPE alpha;
  DATA_TYPE beta;
  volatile DATA_TYPE C[NI][NJ];
  volatile DATA_TYPE A[NI][NK];
  volatile DATA_TYPE B[NK][NJ];
  kernel_gemm (ni, nj, nk,
	       alpha, beta,
	       C,
	       A,
	       B);
  polybench_prevent_dce(print_array(ni, nj,  C));
  return 0;
}
