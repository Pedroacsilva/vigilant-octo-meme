/*CHAD 2019/2020
Autor: Pedro Silva
SAXPY: Multiplicação Z = A * X + Y, onde X, Y Z sao 3 vectores e A um escalar
Implementação em OpenCL*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CL/opencl.h"
#include "AOCL_Utils.h"

using namespace aocl_utils;

#define VECTOR_SIZE 1024

int main(int argc, char const *argv[]){
	printf("CHAD 2019/2020: Implementacao de SAXPY. Autor: Pedro Silva.\n");
	//Alocar memória para vectores XYZ e vector de referencia R
	int * X = (int*)malloc(sizeof(int) * VECTOR_SIZE);
	int * Y = (int*)malloc(sizeof(int) * VECTOR_SIZE);
	int * Z = (int*)calloc(VECTOR_SIZE, sizeof(int));		//Calloc inicializa o vector com 0s. void* calloc (size_t num, size_t size);num:Number of elements to allocate. size: Size of each element.
	int * R = (int*)malloc(sizeof(int) * VECTOR_SIZE);
	//inicializar um escalar aleatório entre 1 e 10
	int A = rand % 10 + 1;
	//Inicializar vectores com inteiros entre 1 er 10, nao esquecer de gerar random seed. Aproveitamos e fazemos tambem o vector de referencia para garantir resultados correctos
	srand((time(NULL)));
	for(int i = 0; i < VECTOR_SIZE; i++){
		X[i] = rand % 10 + 1;
		Y[i] = rand % 10 + 1;
		R[i] = A * X[i] + Y[i];
	}

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

//https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
/*1) Plataformas + devices*/
	cl_platform_id platform_ID = NULL;
	cl_device_id device_ID = NULL;
	cl_uint num_platforms;
	cl_uint num_devices;
	/*Obtain the list of platforms available.
	cl_int clGetPlatformIDs(	cl_uint num_entries,
 	cl_platform_id *platforms,
 	cl_uint *num_platforms)
 	https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clGetPlatformIDs.html*/
	cl_int ret = clGetPlatformIDs(1, &platform_id, &num_platforms);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clGetPlatformIDs(). Devolveu: %i.\n", ret);
	/*Obtain the list of devices available on a platform.
	cl_int clGetDeviceIDs(	cl_platform_id platform,
 	cl_device_type device_type,
 	cl_uint num_entries,
 	cl_device_id *devices,
 	cl_uint *num_devices)*/
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
	cl_mem X_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(int), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (matriz X). Devolveu: %i.\n", ret);
	cl_mem Y_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(int), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (matriz Y). Devolveu: %i.\n", ret);
	cl_mem Z_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE * sizeof(int), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (matriz Z). Devolveu: %i.\n", ret);
 	//6) Transferir dados
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueWriteBuffer.html
	ret = clEnqueueWriteBuffer(command_q, X_buff, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), X, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueWriteBuffer() (X). Devolveu: %i.\n", ret);
	ret = clEnqueueWriteBuffer(command_q, Y_buff, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), X, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueWriteBuffer() (Y). Devolveu: %i.\n", ret);

 	//7) Criar programa
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateProgramFromBinary.html
	std::string binary_file = getBoardBinaryFile("saxpy", device);
	printf("Using AOCX: %s\n", binary_file.c_str());
	cl_program program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);

 	//8) Build
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clBuildProgram.html
	ret = clBuildProgram(program, 1, &device_ID, NULL, NULL, NULL);

 	//9) Criar Kernel e definir argumentos
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clSetKernelArg.html
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateKernel.html
	cl_kernel kernel = clCreateKernel(program, "saxpy", &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateKernel. Devolveu: %i.\n", ret);

	ret = clSetKernelArg(kernel, 0, sizeof(int), (void *)&A);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&X_buff);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&Y_buff);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&Z_buff);

    //10) Correr Kernel
    //https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueNDRangeKernel.html
	size_t global_item_size = VECTOR_SIZE;
    size_t local_item_size = 64;		//Trabalhar em grupos de 64 elementos
    ret = clEnqueueNDRangeKernel(command_q, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    if( ret != CL_SUCCESS)
    	fprintf(stderr, "Erro em clEnqueueNDRangeKernel. Devolveu: %i.\n", ret);

 	//Ler resultados
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueReadBuffer.html
    ret  = clEnqueueReadBuffer(command_q, Z_buff, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), Z, 0, NULL, NULL);

 	//Verificação de resultados
 	int wrong = 0;		//numero de resultados errados
 	for(int i = 0; i < VECTOR_SIZE; i++){
 		if(Z[i] != R[i])
 			wrong++;
 	}
 	printf("Numero total de resultados errados: %i.\n", wrong);

 	//Clean up
 	ret = clFlush(command_q);
 	ret = clFinish(command_q);
 	ret = clReleaseKernel(kernel);
 	ret = clReleaseProgram(program);
 	ret = clReleaseMemObject(X_buff);
 	ret = clReleaseMemObject(Y_buff);
 	ret = clReleaseMemObject(Z_buff);
 	ret = clReleaseCommandQueue(command_q);
 	ret = clReleaseContext(context);
 	free(X);
 	free(Y);
 	free(Z);
 	free(R);
 	return 0;
 }