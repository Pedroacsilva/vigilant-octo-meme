//nvcc -o lab5_2_2 lab5_2_2.cu
/*Author:
Pedro Silva
*/
/*2. Implemente um programa em CUDA que calcule a soma de todos os elementos de um vetor de
tamanho N. Teste para vários valores de N.*/
/*2.2. Implemente uma nova versão otimizada baseada em memória partilhada.*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

__global__ void vectorsum2_2(int * d_buffer, int N){
	//THREAD ID
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	/*Temos N elementos no h_buffer. Vamos partir a soma de todos os elementos como a soma de um elemento com o elemento a sua direita
	Repetir até so termos um elemento (a cada iteração, temos metade dos elementos).*/
	//Comecamos por meter tudo em shared memory
	__shared__ int shared_mem[32];		//32 = Block size
	if(index < N){
		shared_mem[threadIdx.x] = d_buffer[index];
		__syncthreads();
	}
	/*int threads_per_block = 16;
	int distance = 16;
	int primeiro, segundo;*/


	/*//Este ciclo executa enquanto tivermos mais que uma thread e so se a thread ainda estiver no "scope" da soma.
	while(threads_per_block > 1 && threadIdx.x < threads_per_block && index < N / 2){
		//Somar par de elementos  em shared memory
		primeiro = threadIdx.x;
		segundo = threadIdx.x + distance;
		shared_mem[primeiro] = shared_mem[primeiro] + shared_mem[segundo];
		threads_per_block = threads_per_block / 2;
		distance = distance / 2;
		//garantir que todas as threads fizeram a sua soma
		__syncthreads();
	}*/

	//Estou a ter problemas em fazer a reducao, pelo que vou fazer algo mais simples. A thread 0 de cada bloco faz a soma de 32 elementos
		if(threadIdx.x == 0){
			for(int i = 1; i < blockDim.x && (index + i) < N; i++){
				shared_mem[0] += shared_mem[i];
		//		printf("shared_mem[0] = %i. i = %i. .blockIdx.x = %i.\n", shared_mem[0], i, blockIdx.x);
			}
		}

	//A primeira thread de cada bloco deve agora meter o resultado da soma do seu bloco no device buffer de acordo com o indice do seu bloco

		if(threadIdx.x == 0){
			d_buffer[blockIdx.x] = shared_mem[0];
		//printf("A thread do bloco %i escreveu %i.\n", blockIdx.x, shared_mem[0]);
		}
	}		

	int main(){
		printf("Exercicio 2, Lab 5 de CHAD. Soma de todos os elementos de um vector de tamanho N.\nN comeca a 8 (2^3)e duplica até 4096 (2^10).\n");
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
		//Transferir dados do host para device (vector a somar)
			if((error = cudaMemcpy(d_buffer, h_buffer, N * sizeof(int), cudaMemcpyHostToDevice)) != cudaSuccess)
				fprintf(stderr, "Erro a transferir vector para GPU, de dimensao %i. Error = %i.\n", N, error);
		//Inicializar block e grid size
		dim3 BlockSize(32, 1, 1);		//Comecar simples: Blocos de tamanho fixo
		dim3 GridSize(N/32 + 1, 1, 1);
		printf("Gridsize: (%i, %i, %i).\n",GridSize.x, GridSize.y, GridSize.z);	
		vectorsum2_2<<<GridSize, BlockSize>>>(d_buffer, N);
		//Vamos buscar o resultado da soma ao primeiro elemento do d_buffe
		if((error = cudaMemcpy(result, d_buffer, N * sizeof(int), cudaMemcpyDeviceToHost)) != cudaSuccess)
			fprintf(stderr, "Erro a transferir vector do device para host. Error code: %i.\n", error);
		if((error = cudaFree(d_buffer)) != cudaSuccess)
			printf("Erro a libertar memoria no device. Error code: %i.\n", error);
		//Temos agora de somar os resultados de cada bloco
		for(int i = 1; i < N / 32 + 1; i ++)
			result[0] += result[i];
		printf("Resultado: %i.\n", result[0]);
		clock_gettime(CLOCK_MONOTONIC, &end);
		startTime = (start.tv_sec * 1e3) + (start.tv_nsec * 1e-6);
		endTime = (end.tv_sec * 1e3) + (end.tv_nsec * 1e-6);
		printf("Tempo de execução do GPU kernel: %fms.\n", endTime - startTime);
		if((error = cudaFree(d_buffer)) != cudaSuccess)
			printf("Erro a libertar memoria no device para vector. Error code: %i.\n", error);
		free(h_buffer);
		free(result);
	}

	
	return 0;
}