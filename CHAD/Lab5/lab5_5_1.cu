//nvcc -o lab5_5_1 lab5_5_1.cu
//Author: Pedro Silva
/*5. Desenvolva um programa em CUDA que implemente a multiplicação de matrizes
5.1. Implemente uma versão simples (sem optimizações) e compare com a versão sequencial.*/

#include <stdio.h>
#include <stdlib.h>

__global__ void MatrixMul(int * d_A, int * d_B, int* d_C, int N, int M){

	int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	//garantir que estamos no scope do problema
	if(row < M && col < N){
		int index = row * N + col;
		for(int i = 0; i < N; i++){
			d_C[index] += d_A[index] * d_B[index];
		}
	}

}



int main(int argc, char const *argv[])
{
	printf("Exercicio 5.1, Lab 5 de CHAD. Multiplicacao de matrizes (simples) com CUDA.\n");
	int * h_A, * h_B, *d_A, *d_B, *d_C;
	int N = 128;			//Trabalhar com matrizes 128*128
	int M = N;
	//Alocar memoria no host para matriz
	h_A = (int*) malloc(N * N * sizeof(int));
	h_B = (int*) malloc(N * N * sizeof(int));
	//inicializar matrizes A e B
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			h_A[i + j * N] = i;
			h_B[i + j * N] = j;
		}
	}
	//alocar memoria para device
	if(cudaMalloc(&d_A, N * M * sizeof(int)) != cudaSuccess){
		fprintf(stderr, "Erro a alocar memória no device para matriz A.\n");
		return(-1);
	}
	if(cudaMalloc(&d_B, N * M * sizeof(int)) != cudaSuccess){
		fprintf(stderr, "Erro a alocar memória no device para matriz B.\n");
		return(-1);
	}
	if(cudaMalloc(&d_C, N * M * sizeof(int)) != cudaSuccess){
		fprintf(stderr, "Erro a alocar memória no device para matriz C.\n");
		return(-1);
	}
	//transferir dados de host para device
	if(cudaMemcpy(d_A, h_A, N * M * sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess){
		fprintf(stderr, "Erro a transferir matriz A de host para device.\n");
	}
	if(cudaMemcpy(d_B, h_B, N * M * sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess){
		fprintf(stderr, "Erro a transferir matriz B de host para device.\n");
	}
	//definir dimensao de block e grid
	dim3 BlockSize(32,32,1);
	dim3 GridSize(N / 32 + 1, M / 32 + 1, 1);
	//lancar GPU kernel
	MatrixMul<<<GridSize, BlockSize>>>(d_A, d_B, d_C, N, M);
	//Transferir matriz de resultados para host
	if(cudaMemcpy(h_A, d_C, N * M * sizeof(int), cudaMemcpyDeviceToHost) != cudaSuccess){
		fprintf(stderr, "Erro a transferir matriz C de device para host.\n");
	}
	printf("C[%i][%i]: %i.\n", N/2, M/2, h_A[N/2 + M/2 * N]);

	return 0;
}






