//pgcc -lrt -lm -Minfo=all -acc -ta=tesla -o lab3_3.out lab3_3.c
//pgcc -lrt -lm -Minfo=all -acc -ta=radeon -o lab3_3.out lab3_3.c
/*Código acelerado com open acc que calcula o valor médio dos elementos que constituem
 a parte triangular inferior duma matriz quadrada de grandes dimensões*/
//nao estou a ter qualquer speed up e o pgcc nao me deixa compilar para -ta=radeon por algum motivo, mas pelo menos tenho a logica feita
//http://geco.mines.edu/prototype/How_do_you_build_applications/pg/pgcc.html E isto diz que a minha flag ta devia funcionar!
 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main(int argc, char const *argv[])
{
	if(argc != 2){
		printf("Input incorrecto: introduza um número inteiro, n, que define a dimensão da matriz.\n");
		printf("argc: %i.\n", argc);
		return(-1);
	}
	int n = atoi(argv[1]);		//Dimensão da matriz
	float media = 0;			//variável que conterá a média dos valores
	float *restrict a;			//ponteiro para matriz
	struct timespec start, end;
	double startTime, endTime;
/*QUESTAO: Sera melhor alocar um bloco contiguo de memoria e simular uma matriz? ie: float * a = (float*) malloc(memoria), ou será melhor
fazer com que a seja um vector onde cada valor é um ponteiro para um vector da matriz? ie: float ** a =(float**)malloc(memoria para colunas),
iterar por a: for(...){a[i] = (float*) malloc(memoria para linhas)}. Vou fazer um bloco contiguo de memoria porque é mais simples e ter tudo contíguo parece ser simpático*/
	//ALOCAR MEMORIA PARA MATRIZ. a é uma matriz n*n onde cada elemento é um float
	a = (float*) malloc(n*n*sizeof(float));
	//INICIALIZAR MATRIZ a
	for(int i = 0; i < n * n; i++){
		a[i]=sin(i)*sin(i);
	}
	
	//SEQUENCIALMENTE
	//comecar timer
	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < n; i++){		//iterar por todas as linhas
		for(int j = 0; j <= i; j++){	//iterar por todas as colunas com index inferior ou igual a linha
			media = media + a[i * n + j];
		}
	}
	media = media / ((n * (n + 1)) / 2);
	clock_gettime(CLOCK_MONOTONIC, &end);
	//terminar timer
	endTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	startTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	//DEBUG + TIME
	printf("Sequencialmente (matriz %i * %i):\t%f ms.\n", n,n, (endTime - startTime));
	printf("DEBUG: media:\t%f.\n", media);
	media = 0;			//Nao esquecer reinicializar a media
	return(0);
}

