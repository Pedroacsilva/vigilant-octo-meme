//nvcc -o lab5_2_1 lab5_2_1.cu
/*Author:
Pedro Silva
*/
/*2. Implemente um programa em CUDA que calcule a soma de todos os elementos de um vetor de
tamanho N. Teste para vários valores de N.*/
/*2.1. Implemente uma versão simples (sem recorrer a optimizações).*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

__global__ void vectorsum2_1(int * d_buffer, int N){
	//THREAD ID
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	/*Temos N elementos no h_buffer. Vamos partir a soma de todos os elementos como a soma de um elemento com o elemento 16 indices a direita
	Repetir até so termos um elemento (a cada iteração, temos metade dos elementos).*/
	int num_of_threads = N;
	int distance = N / 2;			//Distancia entre elementos a somar
	int primeiro, segundo;
	//Este ciclo executa enquanto tivermos mais que uma thread e so se a thread ainda estiver no "scope" da soma.
	while(num_of_threads > 1 && index < N/2){
		primeiro = index;
		segundo = primeiro + distance;			//na primeira iteracao: 1a thread, index 1, 2a thread, index 3, 3a thread, index 5
		d_buffer[primeiro] = d_buffer[primeiro] + d_buffer[segundo];
		//passou uma iteracao: duplicar a distancia entre elementos a somar e dividir por 2 o numero de threads activas
		distance = distance / 2;
		num_of_threads = num_of_threads / 2;
		//garantir que todas as threads fizeram a sua soma
		__syncthreads();
	}
}		

int main(){
	printf("Exercicio 2, Lab 5 de CHAD. Soma de todos os elementos de um h_buffer de tamanho N.\nN comeca a 8 (2^3)e duplica até 4096 (2^10).\n");
	int *d_buffer, *result, *h_buffer;
	int error;
	struct timespec start, end;
	double startTime, endTime;

	for( int N = 256; N <= 4096; N = N*2){
		printf("--------------------------------------------------------------------------\n");
		printf("Soma de um vector com %i elementos.\n", N);
		clock_gettime(CLOCK_MONOTONIC, &start);
		//alocar memoria no device
		if(cudaMalloc(&d_buffer, sizeof(int) * N) != cudaSuccess){
			fprintf(stderr, "Error allocating memory on device.\n");
			return(-1);
		}
		//alocar memoria no host para h_buffer
		h_buffer = (int*) malloc(N * sizeof(int));
		for(int i = 0; i < N; i++)
			h_buffer[i] = 1;
		//alocar memoria no host para receber o resultado de cada bloco
		result = (int*) malloc(N * sizeof(int));
		//Transferir dados do device para host (vector a somar)
		if((error = cudaMemcpy(d_buffer, h_buffer, N * sizeof(int), cudaMemcpyHostToDevice)) != cudaSuccess)
			fprintf(stderr, "Erro a transferir vector para GPU, de dimensao %i. Error = %i.\n", N, error);
		//Inicializar block e grid size
		dim3 BlockSize(32, 1, 1);		//Comecar simples: Blocos de tamanho fixo
		dim3 GridSize(N/32 + 1, 1, 1);	
		vectorsum2_1<<<GridSize, BlockSize>>>(d_buffer, N);
		//Vamos buscar o resultado da soma ao primeiro elemento do d_buffer
		cudaMemcpy(result, d_buffer, sizeof(int), cudaMemcpyDeviceToHost);
		printf("Resultado da soma de um vector de %i elementos: %i.\n", N, *result);
		if(cudaFree(d_buffer) != cudaSuccess)
			printf("Erro a libertar memoria no device.\n");
		clock_gettime(CLOCK_MONOTONIC, &end);
		startTime = (start.tv_sec * 1e3) + (start.tv_nsec * 1e-6);
		endTime = (end.tv_sec * 1e3) + (end.tv_nsec * 1e-6);
		printf("Tempo de execução do GPU kernel: %fms.\n", endTime - startTime);
		if(cudaFree(d_buffer) != cudaSuccess)
			printf("Erro a libertar memoria no device para vector.\n");
		free(h_buffer);
		free(result);
	}

	
	return 0;
}