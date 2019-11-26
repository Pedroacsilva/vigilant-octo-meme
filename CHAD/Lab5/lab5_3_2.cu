//nvcc -o lab5_3_2 lab5_3_2.cu
/*Author:
Pedro Silva
*/
/*3. Implemente um programa em CUDA que devolva a transposta de uma matriz*/
/*3.2. Implemente uma nova versão otimizada baseada em memória partilhada (​ shared memory ) ​ .*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


__global__ void transposta(int *d_matrix, int *d_out, int N){
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col_t = threadIdx.y + blockIdx.y * blockDim.y;
	int row_t = threadIdx.x + blockIdx.x * blockDim.x;
	__shared__ int shared_mem[32][32];
	if(col<N && row <N)
	{
		shared_mem[threadIdx.y][threadIdx.x] = d_matrix[row*N+col];
		d_out[row_t*N+col_t] = shared_mem[threadIdx.y][threadIdx.x];
	}
}

int main(int argc, char const *argv[])
{
	printf("Exercicio 3, Lab 5 de CHAD. Efectua a transposta duma matriz.\n");
	int *d_matrix, *d_out, *h_matrix, error, M;
	//Start simple. N = M
	for(int N = 64; N <= 512; N = N * 2){
		M = N;
		printf("Transposta duma matriz %i * %i.\n", N, M);
		//alocar memoria para input do device
		if(cudaMalloc(&d_matrix, sizeof(int) * N * M) != cudaSuccess){
			fprintf(stderr, "Error allocating memory on device.\n");
			return(-1);
		}
		//alocar memoria para output do device
		if(cudaMalloc(&d_out, sizeof(int) * N * M) != cudaSuccess){
			fprintf(stderr, "Error allocating memory on device.\n");
			return(-1);
		}
		//alocar memoria para matriz no host
		h_matrix = (int *) malloc(N * M * sizeof(int));
			//inicializar matriz
		for(int i = 0; i < N * M; i++)
			h_matrix[i] = i;

		//dimensionar grid e block sizes
		dim3 BlockSize(32, 32, 1);
		dim3 GridSize(N / 32 + 1, M / 32 + 1, 1);
		//transferir matriz para device.
		if((error = cudaMemcpy(d_matrix, h_matrix, N * M * sizeof(int), cudaMemcpyHostToDevice)))
			fprintf(stderr, "Erro a transferir matriz para device. Error code: %i.\n", error);

		transposta<<<GridSize, BlockSize>>>(d_matrix, d_out, N);
		if((error = cudaMemcpy(h_matrix, d_out, N * M * sizeof(int), cudaMemcpyDeviceToHost)) != cudaSuccess)
			fprintf(stderr, "Erro a transferir matriz do device para host. Error code: %i.\n", error);
		//imprimir uma sub matriz 5*5
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++)
				printf(" %i ", h_matrix[i * N + j]);
			printf("\n");
		}
		if((error = cudaFree(d_matrix)) != cudaSuccess)
			printf("Erro a libertar memoria no device. Error code: %i.\n", error);
		if((error = cudaFree(d_out)) != cudaSuccess)
			printf("Erro a libertar memoria no device. Error code: %i.\n", error);
		free(h_matrix);







	}
	return 0;
}