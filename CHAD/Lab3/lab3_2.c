//pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o lab3_2.out lab3_2.c
//pgcc -lrt -lm -Minfo=all -acc -ta=radeon -o lab3_2.out lab3_2.c
/*2. a) Escreva um programa em OpenACC para calcular a aproximação do logaritmo natural (de base ​ e ​ )
usando os primeiros 10 000 000 de termos da expansão de Maclaurin:
ln(1+x) = x - 1/2 * x² + 1/3 * x ³ - ....*/
/*Nao estou a obter speedup. Penso que a minha flag -ta=radeon esta incorrecta, apesar de http://geco.mines.edu/prototype/How_do_you_build_applications/pg/pgcc.html
confirmar que estou a fazer bem*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main(int argc, char* argv[]){
	if(argc != 3 || atof(argv[2]) < -1 || atof(argv[2]) >= 1){
		printf("Argumentos: n, x. n -> Numero de termos da serie de Maclaurin a calcular.x -> ln(1+x) deverá estar contido em ]-1, 1].\n");
		printf("Argumentos: %i, %f.\n", atoi(argv[1]), atof(argv[2]));
		printf("argc: %i.\n", argc);
		return -1;
	}
	int n = atoi(argv[1]); 		//dimensão de X e Y
	float x = atof(argv[2]);	//define ln(1+x)
	float sum = 0;	//contem o valor ln(1+x) (-1<x<=1)
	printf("n: %i.x: %f.\n", n, x);
	//CLOCK_PROCESS_CPUTIME_ID - Profiling the execution time of loop
	struct timespec start, end;
	sum = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	//SEQUENCIALMENTE
	for(int i = 0; i < n; i++)
		sum = sum + pow(x, i + 1) * pow(-1, i ) / (i + 1);
	clock_gettime(CLOCK_MONOTONIC, &end);
	double initialTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	double finalTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	// DEBUG
	printf("ln(1 + %f):\t%f.\n", x, sum);
	printf("Sequencialmente (%i termos):\t%f ms\n", n, (finalTime - initialTime));
	clock_gettime(CLOCK_MONOTONIC, &start);
	printf("Timer started.\n");
#pragma acc kernels copy(sum), copyin(x, n)//reduction(+:sum)
{		//KERNELS START
	for(int i = 0; i < n; i++)
		sum = sum + pow(x, i + 1) * pow(-1, i ) / (i + 1);
}
//KERNELS END
	clock_gettime(CLOCK_MONOTONIC, &end);
	initialTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	finalTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	// DEBUG
	printf("ln(1 + %f):\t%f.\n", x, sum);
	printf("OpenACC (%i termos):\t%f ms\n", n, (finalTime - initialTime));
	

	//clean up
//	free(a);
	return 0;



}