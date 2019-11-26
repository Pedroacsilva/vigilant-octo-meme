//nvcc -o lab5_1_cpu lab5_1_cpu.cu
//Implementacao em CPU para a conversao em tons de cinza duma imagem RGB
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
//__global__
void colorToGreyScaleConvertion(unsigned char* grayImage, unsigned char
	* rgbImage, int width, int height)
{
	for(int col=0; col<width; col++)
    {
        for(int row=0; row<height; row++)
        {
            // get 1D coordinate for the grayscale image
            int grayOffset = row * width + col;
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
}


int main(int argc, char const *argv[])
{
	printf("Exercicio 1, Lab 5 de CHAD. Leitura e grayscaling de imagens com varias dimensoes (CPU).\n");


	int N, M, channels;
	unsigned char * h_i_rgb, * h_i_gs;
	struct timespec start, end;
	double startTime, endTime;


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
		//Alocar memoria para imagem GS
		h_i_gs = (unsigned char*)malloc(N * M * 1 * sizeof(unsigned char));		//So temos um canal de cinzentos
		//comecar conversao e cronometrar
		clock_gettime(CLOCK_MONOTONIC, &start);
		colorToGreyScaleConvertion(h_i_gs, h_i_rgb, N, M);
		//Gravar imagem em ficheiros
		//int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
		switch(i){
			case 0:
			if(stbi_write_png("image_255_255_grey_cpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 1:
			if(stbi_write_png("image_800_600_grey_cpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 2:
			if(stbi_write_png("image_1920_1080_grey_cpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 3:
			if(stbi_write_png("image_3840_2160_grey_cpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			case 4:
			if(stbi_write_png("image_7680_4320_grey_cpu.png", N, M, 1, h_i_gs, N) == 0)
				fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);
			break;
			default:
			fprintf(stderr, "Erro no switch de imprimir imagem.\n");
			return(-1);
		}

		//Memory cleanup
		stbi_image_free(h_i_rgb);
		free(h_i_gs);
		clock_gettime(CLOCK_MONOTONIC, &end);
		startTime = (start.tv_sec * 1e3) + (start.tv_nsec * 1e-6);
		endTime = (end.tv_sec * 1e3) + (end.tv_nsec * 1e-6);
		printf("Tempo de execução do CPU: %fms.\n", endTime - startTime);
	}
	return 0;
}