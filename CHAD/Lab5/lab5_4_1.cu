//nvcc -o lab5_4_1 lab5_4_1.cu
/*Author:Pedro Silva*/
/*4. Implemente um programa em CUDA que obtenha o histograma da distribuição das intensidades de uma
imagem (representada em ​ uchar​ ) com N píxeis.
4.1. Implemente uma versão simples (sem recorrer a optimizações).*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


__global__ void histograma(unsigned char  *d_image, int * d_histograma, int N, int M){
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int row = threadIdx.y + blockIdx.y * blockDim.y;

	//uma operacao atomica e bloqueante, garante que nenhuma outra thread interfira, prevenindo data hazards.
	//ie: duas threads querem incrementar o valor X ao mesmo tempo. Ambas escrevem X + 1 e sera o valor final, quando deveria ser X + 2.
	if(col<N && row <M){
		atomicAdd(&d_histograma[d_image[row*N+col]], 1);
	}
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

