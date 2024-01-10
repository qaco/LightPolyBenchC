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
#include "3mm.h"
static
void init_array(int ni, int nj, int nk, int nl, int nm,
		DATA_TYPE A[NI][NK],
		DATA_TYPE B[NK][NJ],
		DATA_TYPE C[NJ][NM],
		DATA_TYPE D[NM][NL])
{
  int i, j;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nk; j++)
      A[i][j] = (DATA_TYPE) ((i*j+1) % ni) / (5*ni);
  for (i = 0; i < nk; i++)
    for (j = 0; j < nj; j++)
      B[i][j] = (DATA_TYPE) ((i*(j+1)+2) % nj) / (5*nj);
  for (i = 0; i < nj; i++)
    for (j = 0; j < nm; j++)
      C[i][j] = (DATA_TYPE) (i*(j+3) % nl) / (5*nl);
  for (i = 0; i < nm; i++)
    for (j = 0; j < nl; j++)
      D[i][j] = (DATA_TYPE) ((i*(j+2)+2) % nk) / (5*nk);
}
static
void print_array(int ni, int nl,
		 DATA_TYPE G[NI][NL])
{
  int i, j;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nl; j++) {


    }
}
static
void kernel_3mm(int ni, int nj, int nk, int nl, int nm,
		DATA_TYPE E[NI][NJ],
		DATA_TYPE A[NI][NK],
		DATA_TYPE B[NK][NJ],
		DATA_TYPE F[NJ][NL],
		DATA_TYPE C[NJ][NM],
		DATA_TYPE D[NM][NL],
		DATA_TYPE G[NI][NL])
{
  int i, j, k;
#pragma scop
  /* E := A*B */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	E[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NK; ++k)
	  E[i][j] += A[i][k] * B[k][j];
      }
  /* F := C*D */
  for (i = 0; i < _PB_NJ; i++)
    for (j = 0; j < _PB_NL; j++)
      {
	F[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NM; ++k)
	  F[i][j] += C[i][k] * D[k][j];
      }
  /* G := E*F */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NL; j++)
      {
	G[i][j] = SCALAR_VAL(0.0);
	for (k = 0; k < _PB_NJ; ++k)
	  G[i][j] += E[i][k] * F[k][j];
      }
#pragma endscop
}
int main(int argc, char** argv)
{
  int ni = NI;
  int nj = NJ;
  int nk = NK;
  int nl = NL;
  int nm = NM;
  volatile DATA_TYPE E[NI][NJ];
  volatile DATA_TYPE A[NI][NK];
  volatile DATA_TYPE B[NK][NJ];
  volatile DATA_TYPE F[NJ][NL];
  volatile DATA_TYPE C[NJ][NM];
  volatile DATA_TYPE D[NM][NL];
  volatile DATA_TYPE G[NI][NL];
  polybench_start_instruments;
  kernel_3mm (ni, nj, nk, nl, nm,
	      E,
	      A,
	      B,
	      F,
	      C,
	      D,
	      G);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(ni, nl,  G));
  return 0;
}
