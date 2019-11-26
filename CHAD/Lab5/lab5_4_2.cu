//nvcc -o lab5_4_2 lab5_4_2.cu
/*Author:Pedro Silva*/
/*4. Implemente um programa em CUDA que obtenha o histograma da distribuição das intensidades de uma
imagem (representada em ​ uchar​ ) com N píxeis.
4.2. Implemente uma nova versão otimizada baseada em memória partilhada..*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


__global__ void histograma(unsigned char  *d_image, int * d_histograma, int N, int M){
	//Cada bloco vai fazer um histograma duma sub-imagem
	__shared__ int block_histograma[256];
	__shared__ unsigned char block_image[32][32];
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int row = threadIdx.y + blockIdx.y * blockDim.y;
	//cada thread mete o seu pixel em shared mem
	if(col < N && row < M){
		block_image[threadIdx.x][threadIdx.y] = d_image[row * N + col];
		__syncthreads();
	//cada bloco tem a sua sub imagem. cada thread incrementa o histograma de acordo com o pixel em shared memory
		atomicAdd(&block_histograma[block_image[threadIdx.x][threadIdx.y]], 1);
	}
	//As primeiras 256 threads metem o histograma em shared memory para memoria global que sera transferida para host
	if(threadIdx.x + threadIdx.y * blockDim.x < 256)
		atomicAdd(&d_histograma[threadIdx.x + threadIdx.y * blockDim.x], block_histograma[threadIdx.x + threadIdx.y * blockDim.x]);
}

int main(int argc, char const *argv[]){
	printf("Exercicio 4.1, Lab 5 de CHAD. Histograma das intensidades duma imagem.\n");
	int N, M, channels, * d_histograma, * h_histograma;
	unsigned char * h_image, * d_image;
	//carregar imagem para host
	h_image = stbi_load("image_255_255_grey_gpu.png", &N, &M, &channels, 0);
	if(h_image == NULL)
		fprintf(stderr, "Erro a carregar imagem!\n");
	//alocar memoria para histograma no host
	h_histograma = (int*) malloc(256 * sizeof(int));
	//alocar memoria no device para imagem
	if(cudaMalloc(&d_image, N * M * channels * sizeof(unsigned char)) != cudaSuccess){
		fprintf(stderr, "Erro a alocar memória no device para imagem.\n");
		return(-1);
	}
	if(cudaMalloc(&d_histograma, 256 * sizeof(int)) != cudaSuccess){
		fprintf(stderr, "Erro a alocar memória no device para histograma.\n");
		return(-1);
	}
	//transferir imagem para device
	if(cudaMemcpy(d_image, h_image, N * M * channels * sizeof(unsigned char), cudaMemcpyHostToDevice) != cudaSuccess){
		fprintf(stderr, "Erro a transferir imagem de host para device.\n");
	}
	//dimensionar grid + block
	dim3 BlockSize(32, 32, 1);
	dim3 GridSize(N / 32 + 1, M / 32 + 1, 1);
	//launch kernel
	histograma<<<GridSize, BlockSize>>>(d_image, d_histograma, N, M);
	//transferir dados de device para host
	if(cudaMemcpy(h_histograma, d_histograma, 256 * sizeof(int), cudaMemcpyDeviceToHost) != cudaSuccess){
		fprintf(stderr, "Erro a transferir histograma de device para host.\n");
	}
	//imprimir histograma
	for(int i = 0; i < 255; i ++)
		printf("Intensidade %i: %i.\n", i, h_histograma[i]);
	stbi_image_free(h_image);
	free(h_histograma);
	if(cudaFree(d_image) != cudaSuccess)
		printf("Erro a libertar memoria no device para imagem.\n");
	if(cudaFree(d_histograma) != cudaSuccess)
		printf("Erro a libertar memoria no device para histograma.\n");



	return 0;

}

