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
#include "nussinov.h"
typedef char base;
#define match(b1, b2) (((b1)+(b2)) == 3 ? 1 : 0)
#define max_score(s1, s2) ((s1 >= s2) ? s1 : s2)
static
void init_array (int n,
                 base POLYBENCH_1D(seq,N,n),
		 DATA_TYPE table[N][N])
{
  int i, j;
  //base is AGCT/0..3
  for (i=0; i <n; i++) {
     seq[i] = (base)((i+1)%4);
  }
  for (i=0; i <n; i++)
     for (j=0; j <n; j++)
       table[i][j] = 0;
}
static
void print_array(int n,
		 DATA_TYPE table[N][N])
{
  int i, j;
  int t = 0;
  for (i = 0; i < n; i++) {
    for (j = i; j < n; j++) {


      t++;
    }
  }
}
static
void kernel_nussinov(int n, base POLYBENCH_1D(seq,N,n),
			   DATA_TYPE table[N][N])
{
  int i, j, k;
#pragma scop
 for (i = _PB_N-1; i >= 0; i--) {
  for (j=i+1; j<_PB_N; j++) {
   if (j-1>=0)
      table[i][j] = max_score(table[i][j], table[i][j-1]);
   if (i+1<_PB_N)
      table[i][j] = max_score(table[i][j], table[i+1][j]);
   if (j-1>=0 && i+1<_PB_N) {
     if (i<j-1)
        table[i][j] = max_score(table[i][j], table[i+1][j-1]+match(seq[i], seq[j]));
     else
        table[i][j] = max_score(table[i][j], table[i+1][j-1]);
   }
   for (k=i+1; k<j; k++) {
      table[i][j] = max_score(table[i][j], table[i][k] + table[k+1][j]);
   }
  }
 }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  base seq[N];
  DATA_TYPE table[N][N];
  kernel_nussinov (n, seq, table);
  return 0;
}
