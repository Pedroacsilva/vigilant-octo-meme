/*A equação de Laplace ( ∇ 2T = 0 ) pode ser usada para descrever a variação de temperatura numa placa
metálica. Na prática, esta equação calcula o valor de temperatura de um dado ponto como sendo a média
aritmética dos valores de temperatura da vizinhança:
T (i, j ) = [ T old (i + 1 , j ) + T old (i − 1 , j ) + T old (i, j + 1 ) + T old (i, j − 1 )]/4 .
Neste exercício, a placa metálica é representada por uma grelha 2D e o estado inicial pode ser
aleatoriamente gerado (valores entre 0 e 100 graus). O programa deve iterar sobre a placa metálica e a
condição de paragem é T (i, j ) − T old (i, j ) < 0 .01 .*/
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main(int argc, char const *argv[])
{
	//dimensão da matriz n*m
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	//alocar espaço contíguo para a matriz (mesma duvida que no ex 3.3)
	matrix = (float*) malloc(n * m * sizeof(float));
	//inicializar matriz com numeros aleatorios
	for(int i = 0; i < n * m; i++){
		matrix[i] = rand() % 99 + 1;		//gerar um número aleatório entre 1 e 100
	}
	matrix_old =(float*) calloc(n * n * sizeof(float));		//matriz que segura os valores antigos (calloc inicializa a 0)
	while()
	return 0;
}