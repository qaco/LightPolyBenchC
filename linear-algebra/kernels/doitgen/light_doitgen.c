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
#include "doitgen.h"
static
void init_array(int nr, int nq, int np,
		DATA_TYPE A[NR][NQ][NP],
		DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np))
{
  int i, j, k;
  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++)
	A[i][j][k] = (DATA_TYPE) ((i*j + k)%np) / np;
  for (i = 0; i < np; i++)
    for (j = 0; j < np; j++)
      C4[i][j] = (DATA_TYPE) (i*j % np) / np;
}
static
void print_array(int nr, int nq, int np,
		 DATA_TYPE A[NR][NQ][NP])
{
  int i, j, k;
  for (i = 0; i < nr; i++)
    for (j = 0; j < nq; j++)
      for (k = 0; k < np; k++) {


      }
}
void kernel_doitgen(int nr, int nq, int np,
		    DATA_TYPE A[NR][NQ][NP],
		    DATA_TYPE POLYBENCH_2D(C4,NP,NP,np,np),
		    DATA_TYPE sum[NP])
{
  int r, q, p, s;
#pragma scop
  for (r = 0; r < _PB_NR; r++)
    for (q = 0; q < _PB_NQ; q++)  {
      for (p = 0; p < _PB_NP; p++)  {
	sum[p] = SCALAR_VAL(0.0);
	for (s = 0; s < _PB_NP; s++)
	  sum[p] += A[r][q][s] * C4[s][p];
      }
      for (p = 0; p < _PB_NP; p++)
	A[r][q][p] = sum[p];
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  int nr = NR;
  int nq = NQ;
  int np = NP;
  volatile DATA_TYPE A[NR][NQ][NP];
  volatile DATA_TYPE sum[NP];
  volatile DATA_TYPE C4[NP][NP];
  polybench_start_instruments;
  kernel_doitgen (nr, nq, np,
		  A,
		  C4,
		  sum);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(nr, nq, np,  A));
  return 0;
}
