#include <stdio.h>
#include <stdlib.h>
#include "CL/opencl.h"
#include "AOCL_Utils.h"
#include "stb_image.h"
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION





#define CHANNELS 4 //RGBA

using namespace aocl_utils;

int main(int argc, char const *argv[])
{
	printf("CHAD 2019/2020, conversao duma imagem em tons de cinza. Autor: Pedro Silva.\n");
	int N, M, channels;
	unsigned char * h_i_rgb, * h_i_gs;
	h_i_rgb = stbi_load("image_255_255.png", &N, &M, &channels, 0);
	if(h_i_rgb == NULL)
		fprintf(stderr, "Erro a carregar imagem.\n");
	else
		printf("Dimensoes da iamgem: %i x %i.\tNumero de canais: %i.\n", N, M, channels);
	/*Workflow OpenCL:
	1) Informação sobre plataformas + devices
	2) Escolher devices
	3) Criar contexto OpenCL
	4) Criar Command Q
	5) Criar Buffers
	6) Transferir dados
	7) Criar Program
	8) Carregar kernel e compilar/carregar programa OpenCL précompilado
	9) Definir argumentos do kernel
	10) Correr Kernel	*/
	/*1) Plataformas + devices*/
	cl_platform_id platform_ID = NULL;
	cl_device_id device_ID = NULL;
	cl_uint num_platforms;
	cl_uint num_devices;
	cl_int ret = clGetPlatformIDs(1, &platform_id, &num_platforms);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clGetPlatformIDs(). Devolveu: %i.\n", ret);
	 	/*2) Escolher device*/
	ret = clGetDeviceIDs(platform_ID, CL_DEVICE_TYPE_DEFAULT, 1, &device_ID, &num_devices);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clGetDeviceIDs(). Devolveu: %i.\n", ret);
	 	/*3) Criar context*/
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateContext.html
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateContext(). Devolveu: %i.\n", ret);

 	//4) Criar Command Q
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateCommandQueue.html
	cl_command_queue command_q = clCreateCommandQueue(context, device_id, 0, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateCommandQueue(). Devolveu: %i.\n", ret);
 	//5) Criar buffers
	cl_mem i_rgb_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, N * M * channels * sizeof(unsigned char), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (RGB). Devolveu: %i.\n", ret);
	cl_mem i_gs_buff =  clCreateBuffer(context, CL_MEM_READ_ONLY, N * M * channels * sizeof(unsigned char), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (gs). Devolveu: %i.\n", ret);
	 	//6) Transferir dados
	ret = clEnqueueWriteBuffer(command_q, i_rgb_buff, CL_TRUE, 0, N * M * channels * sizeof(unsigned char), X, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueWriteBuffer() (RGB). Devolveu: %i.\n", ret);
	ret = clEnqueueWriteBuffer(command_q, i_gs_buff, CL_TRUE, 0, N * M * channels * sizeof(unsigned char), X, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueWriteBuffer() (gs). Devolveu: %i.\n", ret);

	 	//7) Criar programa
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateProgramFromBinary.html
	std::string binary_file = getBoardBinaryFile("grayscale", device);
	printf("Using AOCX: %s\n", binary_file.c_str());
	cl_program program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);


 	//8) Build
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clBuildProgram.html
	ret = clBuildProgram(program, 1, &device_ID, NULL, NULL, NULL);
	 	//9) Criar Kernel e definir argumentos
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clSetKernelArg.html
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateKernel.html
	cl_kernel kernel = clCreateKernel(program, "grayscale", &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateKernel. Devolveu: %i.\n", ret);

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&i_rgb_buff);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&i_gs_buff);
	ret = clSetKernelArg(kernel, 2, sizeof(int), (void *)&N);
	ret = clSetKernelArg(kernel, 3, sizeof(int), (void *)&M);

		    //10) Correr Kernel
    //https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueNDRangeKernel.html
	size_t global_item_size[2] = {N, M};
	size_t local_item_size[2] = {64, 64};
	ret = clEnqueueNDRangeKernel(command_q, kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueNDRangeKernel. Devolveu: %i.\n", ret);
    //Transferir resultados (imagem em tons de cinza)
	ret  = clEnqueueReadBuffer(command_q, i_gs_buff, CL_TRUE, 0, N * M * channels * sizeof(unsigned char), h_i_gs, 0, NULL, NULL);
    //gravar imagem
	if(stbi_write_png("image_255_255_grey_gpu.png", N, M, 1, h_i_gs, N) == 0)
		fprintf(stderr, "Erro a imprimir imagem de %i por %i.\n", N, M);

	//Cleanup
	 	ret = clFlush(command_q);
 	ret = clFinish(command_q);
 	ret = clReleaseKernel(kernel);
 	ret = clReleaseProgram(program);
 	ret = clReleaseMemObject(i_rgb_buff);
 	ret = clReleaseMemObject(i_gs_buff);
 	ret = clReleaseCommandQueue(command_q);
 	ret = clReleaseContext(context);
 	free(i_rgb_buff);
 	free(i_gs_buff);



	return 0;
}