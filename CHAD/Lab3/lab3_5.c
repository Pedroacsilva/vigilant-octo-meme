/*A equação de Laplace ( ∇ 2T = 0 ) pode ser usada para descrever a variação de temperatura numa placa
metálica. Na prática, esta equação calcula o valor de temperatura de um dado ponto como sendo a média
aritmética dos valores de temperatura da vizinhança:
T (i, j ) = [ T old (i + 1 , j ) + T old (i − 1 , j ) + T old (i, j + 1 ) + T old (i, j − 1 )]/4 .
Neste exercício, a placa metálica é representada por uma grelha 2D e o estado inicial pode ser
aleatoriamente gerado (valores entre 0 e 100 graus). O programa deve iterar sobre a placa metálica e a
condição de paragem é T (i, j ) − T old (i, j ) < 0 .01 .*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main(int argc, char const *argv[])
{
	if(argc != 3){
		printf("Input incorrecto: introduza dois números inteiros, n m, que definem a dimensão da matriz.\n");
		printf("argc: %i.\n", argc);
		return(-1);
	}
	struct timespec start, end;
	double startTime, endTime;
	//dimensão da matriz n*m
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int size_row = n * sizeof(float);
	int size_col = m * sizeof(float);
	//alocar espaço contíguo para a matriz (mesma duvida que no ex 3.3)
	float * matrix = (float*) malloc(size_row * size_col);
	//inicializar matriz com numeros aleatorios
	for(int i = 0; i < n * m; i++){
		matrix[i] = rand() % 99 + 1;		//gerar um número aleatório entre 1 e 100
	}
	float * matrix_old =(float*) malloc(size_row * size_col);		//matriz que segura os valores antigos (calloc inicializa a 0)

	float delta_max = 0.0, delta;
	clock_gettime(CLOCK_MONOTONIC, &start);
	while(delta_max < 0.01){
		for(int i = 1; i < n - 1; i++){
			for(int j = 1; j < m -1; j++){
				matrix_old[i * size_row + j] = matrix[i * size_row + j];
			}
		}
		//PROBLEMA: ignoramos as primeiras e ultimas linhas e colunas para não irmos "out of bounds". Para resolver isto, podiamos inicializar as matrizes de forma diferente:
		//, alocando [2 * n + 2(m - 2)] * sizeof(float) de bytes a mais e inicializar esses elementos problemáticos a 1(ou a 0)
		for(int i = 1; i < n - 1; i++){
			for(int j = 1; j < m -1; j++){
				//memcpy(matrix_old, matrix, size_row * size_col);		memcpy é de cpu!
				matrix[i * size_row	+ j] = (matrix_old[(i+1) * size_row + j]+ matrix_old[(i-1) * size_row + j] + matrix_old[i * size_row + j + 1] + matrix_old[i * size_row + j - 1])/4;
				//DUVIDA: Para calcular o delta maximo, vou precisar de branches. Mas em GPUs, isso nao e ideal. Qual sera a melhor forma para calcular o maximo duma matriz numa GPU?
			}
		}
		

	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	endTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	startTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	printf("OpenACC (matriz %i * %i):\t%f ms.\n", n,n, (endTime - startTime));
	return 0;
}