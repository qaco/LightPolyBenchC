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
#include "2mm.h"
static
void init_array(int ni, int nj, int nk, int nl,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE A[NI][NK],
		DATA_TYPE B[NK][NJ],
		DATA_TYPE C[NJ][NL],
		DATA_TYPE D[NI][NL])
{
  int i, j;
  *alpha = 1.5;
  *beta = 1.2;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE) ((i*j+1) % ni) / ni;
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE) (i*(j+1) % nj) / nj;
  for (i = 0; i < nj; i++)
    for (j = 0; j < nl; j++)
      C[i][j] = (DATA_TYPE) ((i*(j+3)+1) % nl) / nl;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++)
      D[i][j] = (DATA_TYPE) (i*(j+2) % nk) / nk;
}
static
void print_array(int ni, int nl,
		 DATA_TYPE D[NI][NL])
{
  int i, j;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++) {


    }
}
static
void kernel_2mm(int ni, int nj, int nk, int nl,
		DATA_TYPE alpha,
		DATA_TYPE beta,
		DATA_TYPE tmp[NI][NJ],
		DATA_TYPE A[NI][NK],
		DATA_TYPE B[NK][NJ],
		DATA_TYPE C[NJ][NL],
		DATA_TYPE D[NI][NL])
{
  int i, j, k;
#pragma scop
  /* D := alpha*A*B*C + beta*D */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	tmp[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NK; ++k)
	  tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NL; j++)
      {
	D[i][j] *= beta;
	for (k = 0; k < _PB_NJ; ++k)
	  D[i][j] += tmp[i][k] * C[k][j];
      }
#pragma endscop
}
int main(int argc, char** argv)
{
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int nl = NL;
  DATA_TYPE alpha;
  DATA_TYPE beta;
  volatile DATA_TYPE tmp[NI][NJ];
  volatile DATA_TYPE A[NI][NK];
  volatile DATA_TYPE B[NK][NJ];
  volatile DATA_TYPE C[NJ][NL];
  volatile DATA_TYPE D[NI][NL];
  polybench_start_instruments;
  kernel_2mm (ni, nj, nk, nl,
	      alpha, beta,
	      tmp,
	      A,
	      B,
	      C,
	      D);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(ni, nl,  D));
  return 0;
}
