/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* fdtd-2d.c: this file is part of PolyBench/C */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
/* Include polybench common header. */
#include <polybench.h>
/* Include benchmark-specific header. */
#include "fdtd-2d.h"
/* Array initialization. */
static
void init_array (int tmax,
		 int nx,
		 int ny,
		 DATA_TYPE ex[NX][NY],
		 DATA_TYPE ey[NX][NY],
		 DATA_TYPE hz[NX][NY],
		 DATA_TYPE _fict_[TMAX])
{
  int i, j;
  for (i = 0; i < tmax; i++)
    _fict_[i] = (DATA_TYPE) i;
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++)
      {
	ex[i][j] = ((DATA_TYPE) i*(j+1)) / nx;
	ey[i][j] = ((DATA_TYPE) i*(j+2)) / ny;
	hz[i][j] = ((DATA_TYPE) i*(j+3)) / nx;
      }
}
/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nx,
		 int ny,
		 DATA_TYPE ex[NX][NY],
		 DATA_TYPE ey[NX][NY],
		 DATA_TYPE hz[NX][NY])
{
  int i, j;
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {


    }
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {


    }
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {


    }
}
/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_fdtd_2d(int tmax,
		    int nx,
		    int ny,
		    DATA_TYPE ex[NX][NY],
		    DATA_TYPE ey[NX][NY],
		    DATA_TYPE hz[NX][NY],
		    DATA_TYPE _fict_[TMAX])
{
  int t, i, j;
#pragma scop
  for(t = 0; t < _PB_TMAX; t++)
    {
      for (j = 0; j < _PB_NY; j++)
	ey[0][j] = _fict_[t];
      for (i = 1; i < _PB_NX; i++)
	for (j = 0; j < _PB_NY; j++)
	  ey[i][j] = ey[i][j] - SCALAR_VAL(0.5)*(hz[i][j]-hz[i-1][j]);
      for (i = 0; i < _PB_NX; i++)
	for (j = 1; j < _PB_NY; j++)
	  ex[i][j] = ex[i][j] - SCALAR_VAL(0.5)*(hz[i][j]-hz[i][j-1]);
      for (i = 0; i < _PB_NX - 1; i++)
	for (j = 0; j < _PB_NY - 1; j++)
	  hz[i][j] = hz[i][j] - SCALAR_VAL(0.7)*  (ex[i][j+1] - ex[i][j] +
				       ey[i+1][j] - ey[i][j]);
    }
#pragma endscop
}
int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int tmax = TMAX;
  int nx = NX;
  int ny = NY;
  /* Variable declaration/allocation. */
  volatile DATA_TYPE ex[NX][NY];
  volatile DATA_TYPE ey[NX][NY];
  volatile DATA_TYPE hz[NX][NY];
  volatile DATA_TYPE _fict_[TMAX];
  /* Initialize array(s). */
  /* Start timer. */
  polybench_start_instruments;
  /* Run kernel. */
  kernel_fdtd_2d (tmax, nx, ny,
		  ex,
		  ey,
		  hz,
		  _fict_);
  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;
  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
				    ey,
				    hz));
  /* Be clean. */
  return 0;
}
