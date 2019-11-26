//nvcc -o lab5_1_gpu lab5_1_gpu.cu
//Implementacao em GPU para a conversao em tons de cinza duma imagem RGB
//Autor: Pedro Silva

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
/*stb -> biblioteca para abrir e ler imagens: https://github.com/nothings/stb*/

#define CHANNELS 4		//(RGBA)

// Device code
// We have 3 channels corresponding to RGB
// The input image is encoded as unsigned characters [0, 255]
__global__
void colorToGreyScaleConvertion(unsigned char* grayImage, unsigned char
	* rgbImage, int width, int height)
{
	int Col = threadIdx.x + (blockIdx.x * blockDim.x);
	int Row = threadIdx.y + (blockIdx.y * blockDim.y);
	if (Col < width && Row < height)
	{
		// get 1D coordinate for the grayscale image
		int grayOffset = Row * width + Col;
		// one can think of the RGB image having
		// CHANNEL times columns of the gray scale image
		int rgbOffset = grayOffset * CHANNELS;
		unsigned char r = rgbImage[rgbOffset];
		// red value for pixel
		unsigned char g = rgbImage[rgbOffset + 1];
		// green value for pixel
		unsigned char b = rgbImage[rgbOffset + 2];
		// blue value for pixel
		// perform the rescaling and store it
		// We multiply by floating point constants
		grayImage[grayOffset] = 0.21f * r + 0.71f * g + 0.07f * b;
	}
}


int main(int argc, char const *argv[])
{
	printf("Exercicio 1, Lab 5 de CHAD. Leitura e grayscaling de imagens com varias dimensoes (GPU).\n");
	int N, M, channels;
	unsigned char * h_i_rgb, * h_i_gs;
	unsigned char * d_i_gs, * d_i_rgb;
	struct timespec start, end;
	double startTime, endTime;

	//Vamos testar o device code com imagens de dimensoes: 255x255, 800x600, 1920x1080, 3840x2160, 7680x4320, portanto vamos iterar pelo codigo 5 vezes.

	for(int i = 0; i < 5; i++){
		printf("%i-ésima iteração.\n", i);
		//Definir as dimensoes da imagem, N e M, de acordo com a iteracao, i, do ciclo e load da imagem
		//unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
		// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result
		//int i = 1;
		switch(i){
			case 0:
			h_i_rgb = stbi_load("image_255_255.png", &N, &M, &channels, 0);
			if(h_i_rgb == NULL)
				fprintf(stderr, "Erro a carregar imagem na iteração %i.\n", i);
			printf("Dimensoes da imagem: %i x %i.\tNumero de canais: %i.\n", N, M, channels);
			break;
			case 1:
			h_i_rgb = stbi_load("image_800_600.png", &N, &M, &channels, 0);
			if(h_i_rgb == NULL)
				fprintf(stderr, "Erro a carregar imagem na iteração %i.\n", i);
			printf("Dimensoes da imagem: %i x %i.\tNumero de canais: %i.\n", N, M, channels);
			break;
			case 2:
			h_i_rgb = stbi_load("image_1920_1080.png", &N, &M, &channels, 0);
			if(h_i_rgb == NULL)
				fprintf(stderr, "Erro a carregar imagem na iteração %i.\n", i);
			printf("Dimensoes da imagem: %i x %i.\tNumero de canais: %i.\n", N, M, channels);
			break;
			case 3:
			h_i_rgb = stbi_load("image_3840_2160.png", &N, &M, &channels, 0);
			if(h_i_rgb == NULL)
				fprintf(stderr, "Erro a carregar imagem na iteração %i.\n", i);
			printf("Dimensoes da imagem: %i x %i.\tNumero de canais: %i.\n", N, M, channels);
			break;
			case 4:
			h_i_rgb = stbi_load("image_7680_4320.png", &N, &M, &channels, 0);
			if(h_i_rgb == NULL)
				fprintf(stderr, "Erro a carregar imagem na iteração %i.\n", i);
			printf("Dimensoes da imagem: %i x %i.\tNumero de canais: %i.\n", N, M, channels);
			break;
			default:
			fprintf(stderr, "Erro no switch para inicializar dimensoes da imagem.\n");
			return(-1);
		}
		//Inicializar block e grid size
		dim3 BlockSize(32, 32, 1);		//Comecar simples: Blocos de tamanho fixo
		dim3 GridSize(N / 32 + 1, M / 32 + 1, 1);	

		//Alocar memoria no device para imagem em RGB (entrada) e imagem em grayscale (saida)
		if(cudaMalloc(&d_i_rgb, N * M * channels * sizeof(unsigned char)) != cudaSuccess){
			fprintf(stderr, "Erro a alocar memória no device para imagem RGB na iteração %i.\n", i);
			return(-1);
		}
		if(cudaMalloc(&d_i_gs, N * M * 1 * sizeof(unsigned char)) != cudaSuccess){
			fprintf(stderr, "Erro a alocar memória no device para imagem GS na iteração %i.\n", i);
			return(-1);
		}
		//Transferir dados de device para host (imagem RGB)
		if(cudaMemcpy(d_i_rgb, h_i_rgb, N * M * channels * sizeof(unsigned char), cudaMemcpyHostToDevice) != cudaSuccess){
			fprintf(stderr, "Erro a transferir imagem RGB de host para device.\n");
		}
		//Alocar memoria para receber imagem
		h_i_gs = (unsigned char*)malloc(N * M * 1 * sizeof(unsigned char));		//So temos um canal de cinzentos
		//Lancar kernel do device e cronometrar
		clock_gettime(CLOCK_MONOTONIC, &start);
		colorToGreyScaleConvertion<<<BlockSize, GridSize>>>(d_i_gs, d_i_rgb, N, M);

		//Transferir dados de device->host
		if(cudaMemcpy(h_i_gs, d_i_gs, N * M * 1 * sizeof(unsigned char), cudaMemcpyDeviceToHost) != cudaSuccess){
			fprintf(stderr, "Erro a transferir iamgem GS de device para host.\n");
			return(-1);
		}
		//Gravar imagem em ficheiros
		//int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
		switch(i){
			case 0:
			if(stbi_write_png("image_255_255_grey_gpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 1:
			if(stbi_write_png("image_800_600_grey_gpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 2:
			if(stbi_write_png("image_1920_1080_grey_gpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 3:
			if(stbi_write_png("image_3840_2160_grey_gpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 4:
			if(stbi_write_png("image_7680_4320_grey_gpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			default:
			fprintf(stderr, "Erro no switch de imprimir imagem.\n");
			return(-1);
		}
	/*		switch(i){
				case 0:
				stbi_write_bmp("image_255_255_grey_gpu.bmp", N, M, 1, h_i_gs);
				break;
				case 1:
				stbi_write_bmp("image_800_600_grey_gpu.bmp", N, M, 1, h_i_gs);
				break;
				case 2:
				stbi_write_bmp("image_1920_1080_grey_gpu.bmp", N, M, 1, h_i_gs);
				break;
				case 3:
				stbi_write_bmp("image_3840_2160_grey_gpu.bmp", N, M, 1, h_i_gs);
				break;
				case 4:
				stbi_write_bmp("image_7680_4320_grey_gpu.bmp", N, M, 1, h_i_gs);
				break;
				default:
				fprintf(stderr, "Erro no switch de imprimir imagem.\n");
				return(-1);
			}*/



		//Memory cleanup
				if(cudaFree(d_i_rgb) != cudaSuccess)
					printf("Erro a libertar memoria no device para imagem RGB.\n");
				if(cudaFree(d_i_gs) != cudaSuccess)
					printf("Erro a libertar memoria no device para imagem GS.\n");
				stbi_image_free(h_i_rgb);
				free(h_i_gs);
				clock_gettime(CLOCK_MONOTONIC, &end);
				startTime = (start.tv_sec * 1e3) + (start.tv_nsec * 1e-6);
				endTime = (end.tv_sec * 1e3) + (end.tv_nsec * 1e-6);
				printf("Tempo de execução do GPU kernel: %fms.\n", endTime - startTime);


			}



			return 0;
		}








