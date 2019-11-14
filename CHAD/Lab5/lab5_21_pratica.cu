//nvcc -o lab5_21 lab5_21.cu
/*Author:Pedro Silva*/
/*2. Implemente um programa em CUDA que calcule a soma de todos os elementos de um vetor de
tamanho N. Teste para vários valores de N.*/
/*2.1. Implemente uma versão simples (sem recorrer a optimizações).*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
__global__ void vectorsum2_1(int * device_buffer, int N){
	//THREAD ID
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	/*Temos N elementos no vector. Vamos partir a soma de todos os elementos como a soma de um elemento com o elemento à sua direita.
	Repetir até so termos um elemento (a cada iteração, temos metade dos elementos).*/
	//Assumir que só lançamos um bloco de threads (blockIdx.x = 1 para todas as threads.)
	////int num_of_threads = blockDim.x;
	int distance = blockDim.x;			//Distancia inicial entre elementos a somar
    int num_of_blocks = N / blockDim.x + 1;
	/*Este ciclo executa enquanto tivermos mais que uma thread e so se a thread ainda estiver no "scope" da soma.*/
	while(num_of_blocks > 1 && blockIdx.x < num_of_blocks && index < N){
        
		int primeiro = index * distance * 2;		//na primeira iteracao: 1a thread, index 0, 2a thread, index 2, 3a thread, index 4
		int segundo = primeiro + distance;			//na primeira iteracao: 1a thread, index 1, 2a thread, index 3, 3a thread, index 5
		printf("DEVICE: Thread %i. A somar %i + %i\n", index, device_buffer[primeiro], device_buffer[segundo]);
		device_buffer[primeiro] = device_buffer[primeiro] + device_buffer[segundo];
		//passou uma iteracao: duplicar a distancia entre elementos a somar e dividir por 2 o numero de threads activas
        distance *= 2;
        num_of_blocks--;
	}
	int num_of_threads = blockDim.x/2;
	if(num_of_blocks == 1 && num_of_threads > 1){
        int primeiro = index * 2;
        int segundo = primeiro + 1;
        device_buffer[primeiro] = primeiro + segundo;
        num_of_threads /=2;
    }
}		

int main(){
    struct timespec start_device, end_device, start_host, end_host;
    double initialTime, finalTime;
    int result;

    for(int N = 8; N <= 1024; N = N*2){
        printf("N = %i.\n", N);
        int *device_buffer = NULL;
        int *host_buffer = NULL;
		int err = cudaMalloc(&device_buffer, sizeof(int) * N);
		if(err != cudaSuccess){
			fprintf(stderr, "Error allocating memory on device.\n");
			return(-1);
		}
		//Inicializar vector N:
		
        host_buffer = (int*)malloc(N * sizeof(int));
        for(int i = 0; i < N; i++)
            host_buffer[i] = i;
        //DEVICE
        //enviar dados para device
        cudaMemcpy(device_buffer, host_buffer, N * sizeof(int), cudaMemcpyHostToDevice);
        //comecar computacao
        clock_gettime(CLOCK_MONOTONIC, &start_device);
		vectorsum2_1<<< N/256 + 1, 256>>>(device_buffer, N);
        clock_gettime(CLOCK_MONOTONIC, &end_device);
        //cronometrar
        initialTime = (start_device.tv_sec*1e3) + (start_device.tv_nsec*1e-6);
        finalTime = (end_device.tv_sec*1e3) + (end_device.tv_nsec*1e-6);
		/*Vamos buscar o resultado da soma ao primeiro elemento do device_buffer*/
		cudaMemcpy(&result, device_buffer, sizeof(int), cudaMemcpyDeviceToHost);
		printf("DEVICE: Resultado da soma de um vector de %i elementos: %i.\n", N, result);
        printf("DEVICE: Tempo de execução (device): \t%fms.\n", (finalTime - initialTime));
        //HOST
        result = 0;
        clock_gettime(CLOCK_MONOTONIC, &start_host);
        for(int i = 0; i < N; i++)
            result += host_buffer[i];
        clock_gettime(CLOCK_MONOTONIC, &end_host);
        initialTime = (start_host.tv_sec*1e3) + (start_host.tv_nsec*1e-6);
        finalTime = (end_host.tv_sec*1e3) + (end_host.tv_nsec*1e-6);
        printf("HOST: Resultado da soma de um vector de %i elementos: %i.\n", N, result);
        printf("HOST: Tempo de execução (device): \t%fms.\n", (finalTime - initialTime));
        cudaFree(device_buffer);
        free(host_buffer);
        return 0;       //TEMPORARIO. So quero testar para N = 8
	}
	
	return 0;
}


