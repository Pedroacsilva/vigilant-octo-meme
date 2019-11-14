// gcc -o vecAdd_seq.out vecAdd_seq.c -lrt -lm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main( int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("./vecAdd_seq num_elem\n");
		return 0;
	}
	int n;
	n=atoi(argv[1]);
	// vector declaration
	float *a;
	float *b;
	float *c;
	// Size, in bytes, of each vector
	size_t bytes=n*sizeof(float);
	// Allocate memory for each vector
	a=(float*)malloc(bytes);
	b=(float*)malloc(bytes);
	c=(float*)malloc(bytes);
	// Initialize content of input vector a
	int i;
	for(i=0; i<n; i++)
	{
		a[i]=sin(i)*sin(i);
		b[i]=cos(i)*sin(i);
	} 
	//CLOCK_PROCESS_CPUTIME_ID - Profiling the execution time of loop
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	// c=a+b
	for(i=0; i<n; i++)
	{
		c[i]=a[i]+b[i];
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	//
	double initialTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	double finalTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	printf("Sequential (# elements=%i):\t%f ms\n", n, (finalTime - initialTime));
	// Release memory
	free(a);
	free(b);
	free(c);
	//
	return 0;
}
