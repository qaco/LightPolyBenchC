/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
typedef int bool;
const int true = 1;
const int false = 0;
#if ! defined SCALAR_REPLACEMENT
#define SCALAR_REPLACEMENT 0
#endif
#if ! defined CHECK_DEPENDENCES
#define CHECK_DEPENDENCES false
#endif
#if defined NDEBUG
#define eassert(EXPR)   1
#else
#define eassert(EXPR)	eassert_func(__STRING(EXPR), EXPR)
void eassert_func(char *expr, bool value)
{
    if (!value) {

	exit(1);
	// printf("assertion failed: %s\n", expr);
    }
}
#endif
//#define MAX_SIZE 16307
#if ! defined FOUR_WAY_MAX_WITH_REDUNDANCY
#define FOUR_WAY_MAX_WITH_REDUNDANCY false
#endif
#if FOUR_WAY_MAX_WITH_REDUNDANCY
#define ZERO_IF_NO_REDUNDANCY 1
#else
#define ZERO_IF_NO_REDUNDANCY 0
#endif
#if ! defined PRINT_SIZE
#define PRINT_SIZE  48  
#endif
#if ! defined VERBOSE
#define VERBOSE       false
#endif
#if ! defined VERBOSE_OUT
#define VERBOSE_OUT stdout
#endif
#if VERBOSE
#if ! defined REALLY_VERBOSE
#define REALLY_VERBOSE false
#endif
#endif
#if ! defined SEED
#define SEED 42
#endif
#define SLOWER (CHECK_DEPENDENCES | VERBOSE)
double cur_time(void)
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	return tv.tv_sec + tv.tv_usec*1.0e-6;
}
// for bases, use 0, 1, 2, 3, with (0, 3) and (1, 2) being matches
char *base_print[4] = { "A", "C", "G", "U" };
typedef int base;	// could also try char, short
typedef int score;	// could use uint, short, anything that can count high enough
inline score match(base b1, base b2)
{
	return (b1+b2) == 3 ? 1 : 0;
}
inline score max_score(score s1, score s2)
{
	return (s1 >= s2) ? s1 : s2;
}
base  seq[MAX_SIZE];
score   N_array[MAX_SIZE][MAX_SIZE];
#define debug_N(x, y)	(N_array[x][y])  
#if ! CHECK_DEPENDENCES
#define N(x, y) 	(eassert(0 <= x && x < size && 0 <= y && y < size), N_array[x][y])
#if SCALAR_REPLACEMENT
#define MAX_N_DECLS()	int max_tmp_i, max_tmp_j; score max_tmp
#define MAX_N_START(x,y)	((max_tmp_i=x), (max_tmp_j=y), (max_tmp = 0))
#define MAX_N(x, y, v)	(eassert(max_tmp_i==x && max_tmp_j==y), eassert(0 <= x && x < size && 0 <= y && y < size), (max_tmp = max_score(max_tmp, v)))
#define MAX_N_END(x,y)	(eassert(max_tmp_i==x && max_tmp_j==y), ((N_array[x][y]) = max_score(N_array[x][y], max_tmp)))
#else  
#define MAX_N(x, y, v)	(eassert(0 <= x && x < size && 0 <= y && y < size), ((N_array[x][y]) = max_score(N_array[x][y], v)))
#endif 
#else  
bool    N_array_has_been_read[MAX_SIZE][MAX_SIZE];
#define N(x, y) 	(eassert(0 <= x && x < size && 0 <= y && y < size), \

			 (N_array_has_been_read[x][y] = (true)), \
			 N_array[x][y]+0)
#if SCALAR_REPLACEMENT  
#error("Not yet ready to do scalar replacement and check_deps at the same time :-(")
#else  
#define MAX_N(x, y, v)	(eassert(0 <= x && x < size && 0 <= y && y < size), \
			 eassert(!N_array_has_been_read[x][y]), \
			 (N_array[x][y] = max_score(N_array[x][y], v)))
#endif 
#endif
#if ! SCALAR_REPLACEMENT
#define MAX_N_DECLS()
#define MAX_N_START(x,y)
#define MAX_N_END(x,y) 
#endif
int getint(char *prompt)
{
#if VERBOSE_OUT == stderr
	char *terminate = "\n";
#else
	char *terminate = "";
#endif
	int result;
	int i=0;
	while (

		result = scanf("%d", &i),
		result != 1 && result != EOF
	) {

	}
	if (result == 1) {
		return i;
	} else {

		exit(1);
	}
}
int main(int argc, char *argv[])
{
#if ! SLOWER
	double start_time, end_time; // , speed;
#endif
#if ! defined size
 	int size = getint("Enter length of random mRNA sequence (2200 is average for human mRNA): ");  // Average (human) mRNA length is 2200; there's one that's 5000, though
#endif
        int i, j, k=-1;
	MAX_N_DECLS();
	char *options;
#if VERBOSE
#if CHECK_DEPENDENCES
	options = " [DV]";
#else
	options = " [V]";
#endif
#else
#if CHECK_DEPENDENCES
	options = " [D]";
#else
	options = "";
#endif
#endif
	printf("Running Nussinov RNA algorithm%s for sequence of length %d, random data with seed %d.\n",
	       options, size, SEED);
	if (size > MAX_SIZE) {

		exit(1);
	}
	srand(SEED);
	for (i = 0; i < size; i++)
		seq[i] = rand() % 4;
#if ! SLOWER
	start_time = cur_time();
#endif
// "OPTION 1"
#pragma scop
	for (i = size-1; i >= 0; i--) {
		for (j=i+1; j<size; j++) {
#if VERBOSE

#endif
			MAX_N_START(i, j);
#if FOUR_WAY_MAX_WITH_REDUNDANCY
			if (j-1>=0)   MAX_N(i, j, N(i, j-1));
			if (i+1<size) MAX_N(i, j, N(i+1, j));
#endif
			if (j-1>=0 && i+1<size) {
			  if (i<j-1) MAX_N(i, j, N(i+1, j-1)+match(seq[i], seq[j]));  
			  else       MAX_N(i, j, N(i+1, j-1));
			}
			{
			int k;  
			for (k=i+ZERO_IF_NO_REDUNDANCY; k<j; k++) {
#if VERBOSE

#endif
				MAX_N(i, j, N(i, k)+N(k+1, j));
			}
			} 
			MAX_N_END(i, j);
		}
	}
#pragma endscop
#if !SLOWER
	end_time = cur_time();
	printf("done.\nTime elapsed: %fs\n", end_time-start_time);
#endif
	printf("N(0, size-1) = %d\n", N(0, size-1));
	if (size <= PRINT_SIZE) {
		for (i=0; i<size; i++)
			printf("%3s ", base_print[seq[i]]);
		printf("\n");
		for(i = 0; i < size; i++) {
			for(j = 0; j < size; j++) printf("%3d ", debug_N(i, j));
			printf("\n");
		}
	}
	eassert(k == -1);
	return 0;
}
