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
#include "bicg.h"
static
void init_array (int m, int n,
		 DATA_TYPE A[N][M],
		 DATA_TYPE r[N],
		 DATA_TYPE p[M])
{
  int i, j;
  for (i = 0; i < m; i++)
    p[i] = (DATA_TYPE)(i % m) / m;
  for (i = 0; i < n; i++) {
    r[i] = (DATA_TYPE)(i % n) / n;
    for (j = 0; j < m; j++)
      A[i][j] = (DATA_TYPE) (i*(j+1) % n)/n;
  }
}
static
void print_array(int m, int n,
		 DATA_TYPE s[M],
		 DATA_TYPE q[N])
{
  int i;
  for (i = 0; i < m; i++) {


  }
  for (i = 0; i < n; i++) {


  }
}
static
void kernel_bicg(int m, int n,
		 DATA_TYPE A[N][M],
		 DATA_TYPE s[M],
		 DATA_TYPE q[N],
		 DATA_TYPE p[M],
		 DATA_TYPE r[N])
{
  int i, j;
#pragma scop
  for (i = 0; i < _PB_M; i++)
    s[i] = 0;
  for (i = 0; i < _PB_N; i++)
    {
      q[i] = SCALAR_VAL(0.0);
      for (j = 0; j < _PB_M; j++)
	{
	  s[j] = s[j] + r[i] * A[i][j];
	  q[i] = q[i] + A[i][j] * p[j];
	}
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  int m = M;
  volatile DATA_TYPE A[N][M];
  volatile DATA_TYPE s[M];
  volatile DATA_TYPE q[N];
  volatile DATA_TYPE p[M];
  volatile DATA_TYPE r[N];
  polybench_start_instruments;
  kernel_bicg (m, n,
	       A,
	       s,
	       q,
	       p,
	       r);
  polybench_stop_instruments;
  polybench_print_instruments;
  polybench_prevent_dce(print_array(m, n, s, q));
  return 0;
}
