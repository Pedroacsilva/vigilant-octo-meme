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
//NOTA : Podemos fazer um melhor data management: se ordenarmos os elementos da matriz a por forma a que os primeiros n*(n+1)/2 valores sejam 
//parte triangular inferior da nossa matriz, entao passamos menos dados para o device: podemos fazer
//data copyin(a[0:n*(n+1)/2]) em vez de data copyin(a[0:n²]), menos dados transferidos, menos overhead 
	//Começar timer
	clock_gettime(CLOCK_MONOTONIC, &start);
	/*Queremos iterar pelos valores triangulares inferiores. Seja a[i,j] um valor da matriz onde i é a linha e j a coluna.
	Queremos iterar por todos os valores em que i <= j*/
#pragma acc data copyin(a[0:n*n]) copy(media)
{			//DATA MANAGEMENT
#pragma acc parallel loop reduction(+:media)
	//num_gangs(256) vector_length(1024), nao sei avaliar a dimensao de gangs e threads, provavelmente deverão estar em função de n
	//Podemos melhorar o paralelismo. A dimensão do problema é a soma de n(n+1)/2 termos, portanto devemos ter esse mesmo
	//numero de threads na nosa GPU (grid * block size = dimensao do problema)
	for(int i = 0; i < n; i++){		//iterar por todas as linhas
		for(int j = 0; j <= i; j++){	//iterar por todas as colunas com index inferior ou igual a linha
			media = media + a[i * n + j];
		}
	}
}			//DATA MANAGEMENT END
	//Quantos valores somámos? É uma série gaussiana(1 + 2 + 3 + ... + n), portanto sum = n*(n+1)/2
	media = media / ((n * (n + 1)) / 2);
	clock_gettime(CLOCK_MONOTONIC, &end);
	//timer end
	endTime=(end.tv_sec*1e3)+(end.tv_nsec*1e-6);
	startTime=(start.tv_sec*1e3)+(start.tv_nsec*1e-6);
	//DEBUG
	printf("OpenACC (matriz %i * %i):\t%f ms.\n", n,n, (endTime - startTime));
	printf("DEBUG: media:\t%f.\n", media);

	return 0;

}

