//pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o lab3_1seq.out lab3_1seq.c
/*SAXPY (Single-Precision A.X plus Y) é uma operação muito comum em computação e é definida da
seguinte forma: ​ y=a.x+y ​ , onde ​ a é um valor escalar, ​ x e ​ y são vetores com N elementos cada.
Implemente, usando directivas OpenACC, uma rotina que execute esta operação. Compare a performance
com a equivalente versão sequencial.*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main(int argc, char* argv[]){
		if(argc != 3)
	{
		printf("Argumentos: a, n. a é o valor escalar a multiplicar os vectores X e Y com n elementos cada.\n");
		return -1;
	}
	int n, a;
	a = atoi(argv[1]);		//escalar
	n = atoi(argv[2]); 		//dimensão de X e Y
	printf("a: %i. n: %i.\n", a, n);
	// vector declaration
	float *restrict x;
	float *restrict y;
	// Allocate memory for each vector
	x = malloc(n * sizeof(float));
	y = malloc(n * sizeof(float));
	printf("Vectores alocados.\n");
	// Inicializar os vectores x e y
	int i;
	for(int i = 0; i < n; i++)
	{
		x[i]=sin(i)*sin(i);
		y[i]=cos(i)*sin(i);
	}
	printf("Vectores inicializados.\n");
	//CLOCK_PROCESS_CPUTIME_ID - Profiling the execution time of loop
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	printf("Timers declared.\n");
	for(int i = 0; i < n; i++){
		y[i] = a * x[i] + y[i];
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	//
	double initialTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	double finalTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	// DEBUG
	if( n > 100)
		printf("DEBUG: y[100]=%f\n", y[100]);
	//
	printf("Sequencialmente (%ix%i elemens):\t%f ms\n", n,n, (finalTime - initialTime));
	// Release memory
	free(x);
	free(y);
	//
	return 0;



}