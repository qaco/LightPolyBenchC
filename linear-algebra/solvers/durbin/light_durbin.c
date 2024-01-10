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
#include "durbin.h"
static
void init_array (int n,
		 DATA_TYPE r[N])
{
  int i, j;
  for (i = 0; i < n; i++)
    {
      r[i] = (n+1-i);
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
void kernel_durbin(int n,
		   DATA_TYPE r[N],
		   DATA_TYPE y[N])
{
 DATA_TYPE z[N];
 DATA_TYPE alpha;
 DATA_TYPE beta;
 DATA_TYPE sum;
 int i,k;
#pragma scop
 y[0] = -r[0];
 beta = SCALAR_VAL(1.0);
 alpha = -r[0];
 for (k = 1; k < _PB_N; k++) {
   beta = (1-alpha*alpha)*beta;
   sum = SCALAR_VAL(0.0);
   for (i=0; i<k; i++) {
      sum += r[k-i-1]*y[i];
   }
   alpha = - (r[k] + sum)/beta;
   for (i=0; i<k; i++) {
      z[i] = y[i] + alpha*y[k-i-1];
   }
   for (i=0; i<k; i++) {
     y[i] = z[i];
   }
   y[k] = alpha;
 }
#pragma endscop
}
int main(int argc, char** argv)
{
  int n = N;
  volatile DATA_TYPE r[N];
  volatile DATA_TYPE y[N];
  kernel_durbin (n,
		 r,
		 y);
  polybench_prevent_dce(print_array(n, y));
  return 0;
}
