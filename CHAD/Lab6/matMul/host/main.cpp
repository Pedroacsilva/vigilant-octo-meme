/*CHAD 2019/2020
Autor: Pedro Silva
SAXPY: Multiplicação de duas matrizes quadradadas
Implementação em OpenCL*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CL/opencl.h"

#define MATRIX_SIZE 1024
#define TOTAL_LENGTH 1048576			//1024^2


int main(int argc, char const *argv[]){
	printf("CHAD 2019/2020. Multiplicacao de duas matrizes quadadas de dimensao 1024 * 1024. Autor: Pedro Silva.\n")
	//Alocar memoria para matrizes X Y Z e de verificação R
	int * X = (int*)malloc(sizeof(int) * TOTAL_LENGTH);
	int * Y = (int*)malloc(sizeof(int) * TOTAL_LENGTH);
	int * Z = (int*)calloc(TOTAL_LENGTH, sizeof(int));		//Calloc inicializa o vector com 0s. void* calloc (size_t num, size_t size);num:Number of elements to allocate. size: Size of each element.
	int * R = (int*)calloc(TOTAL_LENGTH, sizeof(int));

	//Inicializar matrizes com valores aleatorios entre 1 e 10
	srand((time(NULL)));		//seed
	for(int i = 0; i < TOTAL_LENGTH; i++){
		X[i] = rand % 10 + 1;
		Y[i] = rand % 10 + 1;
	}
	//Calcular matriz de referencia
	for(int i = 0; i < MATRIX_SIZE; i++){
		for(int j = 0; j < MATRIX_SIZE; j++){
			for(int k = 0; k < MATRIX_SIZE; k++)
				R[i + j * MATRIX_SIZE] += A[k + j * MATRIX_SIZE] * B[i + k * MATRIX_SIZE];
		}
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
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateContext(). Devolveu: %i.\n", ret);
 	 	//4) Criar Command Q
	cl_command_queue command_q = clCreateCommandQueue(context, device_id, 0, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateCommandQueue(). Devolveu: %i.\n", ret);
 	 	//5) Criar buffers
	cl_mem X_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, TOTAL_LENGTH * sizeof(int), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (vector X). Devolveu: %i.\n", ret);
	cl_mem Y_buff = clCreateBuffer(context, CL_MEM_READ_ONLY, TOTAL_LENGTH * sizeof(int), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (vector Y). Devolveu: %i.\n", ret);
	cl_mem Z_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, TOTAL_LENGTH * sizeof(int), NULL, &ret);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clCreateBuffer() (vector Z). Devolveu: %i.\n", ret);
 	 	//6) Transferir dados
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueWriteBuffer.html
	ret = clEnqueueWriteBuffer(command_q, X_buff, CL_TRUE, 0, TOTAL_LENGTH * sizeof(int), X, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueWriteBuffer() (X). Devolveu: %i.\n", ret);
	ret = clEnqueueWriteBuffer(command_q, Y_buff, CL_TRUE, 0, TOTAL_LENGTH * sizeof(int), X, 0, NULL, NULL);
	if( ret != CL_SUCCESS)
		fprintf(stderr, "Erro em clEnqueueWriteBuffer() (Y). Devolveu: %i.\n", ret);
 	 	//7) Criar programa
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clCreateProgramWithSource.html
	std::string binary_file = getBoardBinaryFile("matMul", device);
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
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&X_buff);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&Y_buff);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&Z_buff);
	    //10) Correr Kernel
    //https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueNDRangeKernel.html
	size_t global_item_size[2] = {VECTOR_SIZE, VECTOR_SIZE};
    size_t local_item_size[2] = {64, 64};
    ret = clEnqueueNDRangeKernel(command_q, kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    if( ret != CL_SUCCESS)
    	fprintf(stderr, "Erro em clEnqueueNDRangeKernel. Devolveu: %i.\n", ret);

 	//Ler resultados
 	//https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueReadBuffer.html
    ret  = clEnqueueReadBuffer(command_q, Z_buff, CL_TRUE, 0, TOTAL_LENGTH * sizeof(int), Z, 0, NULL, NULL);
     	//Verificação de resultados
 	int wrong = 0;		//numero de resultados errados
 	for(int i = 0; i < TOTAL_LENGTH; i++){
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