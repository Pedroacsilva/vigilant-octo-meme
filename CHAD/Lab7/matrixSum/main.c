/*CHAD 2019_2020
 * autor: Pedro Silva
 * Programa que implementa a soma de 2 matrizes usando OpenCL em GPU.*/


// gcc -I /usr/local/cuda-10.1/include/ main.c -lm -lOpenCL -o opencl_matrixSum.o
#include <stdlib.h>
#include <stdio.h>
#include <CL/cl.h>
#include <math.h>
#include <time.h>

#define MAX_SOURCE_SIZE (0x100000)

struct timespec t0, t1;

int main()
{
	// Number of elements in each vector
	unsigned int N=256;
    unsigned int M=256;
	size_t mat_size_bytes=N*M*sizeof(int);

	int cl_error;
	// 1st step: Get OpenCL platforms available in the machine
	cl_platform_id platform;
	cl_error=clGetPlatformIDs(1, &platform, NULL);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: cl_getPlatformIDs; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// Optional: Get OpenCL platform name
	size_t platform_name_size=0;
  cl_error=clGetPlatformInfo(platform, CL_PLATFORM_NAME,  0, NULL, &platform_name_size);
  if(cl_error!=CL_SUCCESS)
  {
    printf("\nERROR: clGetPlatformInfo (1); ERROR code:%i", cl_error);
    printf("\n\n");
		return 0;
  }
  char *platform_name=(char *)malloc(sizeof(char)*(platform_name_size+1));
  cl_error=clGetPlatformInfo(platform, CL_PLATFORM_NAME, platform_name_size, platform_name, NULL);
  if(cl_error!=CL_SUCCESS)
  {
    printf("\nERROR: clGetPlatformInfo (2); ERROR code:%i", cl_error);
    printf("\n\n");
		return 0;
  }
  printf("\nOpenCL platform available: %s", platform_name);

	// 2nd step: Get OpenCL devices associated with platform
	cl_device_id device_id;
	cl_error=clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clGetDeviceIDs; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// Optional: Get OpenCL device name
  size_t device_name_size=0;
  cl_error=clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, NULL, &device_name_size);
  if(cl_error!=CL_SUCCESS)
  {
    printf("\nERROR: clGetDeviceInfo (1); ERROR code:%i", cl_error);
    printf("\n\n");
		return 0;
  }
  char *device_name=(char *)malloc(sizeof(char)*(device_name_size+1));
  cl_error=clGetDeviceInfo(device_id, CL_DEVICE_NAME, device_name_size, device_name, NULL);
  if(cl_error!=CL_SUCCESS)
  {
    printf("\nERROR: clGetDeviceInfo (2); ERROR code:%i", cl_error);
    printf("\n\n");
		return 0;
  }
  printf("\nOpenCL device selected: %s",device_name);

	// 3rd step: Create an OpenCL context
	cl_context context;
	context=clCreateContext(NULL, 1, &device_id, NULL, NULL, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateContext; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}

	// 4th step: Create a OpenCL command queue (all OpenCL commands are issued/executed in order to/from the command queue)
	cl_command_queue command_queue;
	command_queue=clCreateCommandQueue(context, device_id, 0, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateCommandQueue; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	} 

	// 5th step: Create a OpenCL program
	// 5.1. Read vectorSum.cl to a string
	size_t file_size=0;
	char *kernel_string=NULL;
	FILE *fp=fopen("matrixSum.cl", "rb");
	if(fp==NULL)
	{
		printf("\nERROR: fopen(%s)", "matrixSum.cl");
		printf("\n\n");
		return 0;
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		file_size=ftell(fp);
		rewind(fp);
		kernel_string=(char *)malloc(sizeof(char)*(file_size+1));
		kernel_string[file_size]='\0';
		fread(kernel_string, sizeof(char), file_size, fp);
		fclose(fp);
		printf("\nOpenCL kernel:\n%s\n", kernel_string);
	}
	 	// 5.2. Create OpenCL program object
	cl_program program;
	program=clCreateProgramWithSource(context, 1, (const char **)&kernel_string, (const size_t *)&file_size, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateProgramWithSource; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}

	// 6th step: Build the OpenCL program executable
	cl_error=clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clBuildProgram; ERROR code:%i", cl_error);
		// Get build log information
    // Get size of build log
    size_t buildLogSize;
    cl_error=clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &buildLogSize);
    // Get build log
    char* buildLogBuffer = (char*)malloc(buildLogSize);
    cl_error=clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, buildLogSize, buildLogBuffer, NULL);
    printf("\n################# Building log #################");
    printf("\n%s", buildLogBuffer);
    printf("\n############# End of Building log ##############");
    fflush(stdout);
    free(buildLogBuffer);
    printf("\n\n");
		return 0;
	}

	// 7th step: Create the OpenCL kernel
	cl_kernel kernel;
	kernel=clCreateKernel(program, "matrixSum", &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateKernel; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}

	// 8th step: Assign the kernel arguments
	// 8.1. Define host buffers
	int *host_buffer_m1=(int *)malloc(mat_size_bytes);
    int *host_buffer_m2=(int *)malloc(mat_size_bytes);
    int *host_buffer_m3=(int *)malloc(mat_size_bytes);
    //inicializar dados dos vectores v1 e v2
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            host_buffer_m1[i * N + j] = 2;
            host_buffer_m2[i * N + j] = 3;
            host_buffer_m3[i * N + j] = 0;
        }
    }
    printf("host_buffer_m1[5][5] = %i\thost_buffer_m2[5][5] = %i\thost_buffer_m3[5][5] = %i\t.\n", host_buffer_m1[5 + 5 * M], host_buffer_m2[5 + 5 * M], host_buffer_m3[5 + 5 * M]);
	cl_mem device_buffer_m1=clCreateBuffer(context, CL_MEM_READ_ONLY, mat_size_bytes, NULL, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateBuffer device_buffer_m1; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
    cl_mem device_buffer_m2=clCreateBuffer(context, CL_MEM_READ_ONLY, mat_size_bytes, NULL, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateBuffer device_buffer_m2; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	cl_mem device_buffer_m3=clCreateBuffer(context, CL_MEM_READ_WRITE, mat_size_bytes, NULL, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateBuffer device_buffer_m3; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// 8.4. Set arguments to the correspondent kernel
	cl_error=clSetKernelArg(kernel, 0, sizeof(cl_mem), &device_buffer_m1);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg device_buffer_m1; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	cl_error=clSetKernelArg(kernel, 1, sizeof(cl_mem), &device_buffer_m2);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg device_buffer_m2; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
    cl_error=clSetKernelArg(kernel, 2, sizeof(cl_mem), &device_buffer_m3);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg device_buffer_m3; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
    cl_error=clSetKernelArg(kernel, 3, sizeof(int), &N);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg N; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
    cl_error=clSetKernelArg(kernel, 4, sizeof(int), &M);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg M; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	//Transferir buffers de dados de host->device
	cl_error = clEnqueueWriteBuffer(command_queue, device_buffer_m1, CL_FALSE, 0, sizeof(int) * N * M, host_buffer_m1, 0, NULL, NULL);
    if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clEnqueueWriteBuffer host_buffer_m1 -> device_buffer_m1; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
		cl_error = clEnqueueWriteBuffer(command_queue, device_buffer_m2, CL_FALSE, 0, sizeof(int) * N * M, host_buffer_m2, 0, NULL, NULL);
    if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clEnqueueWriteBuffer host_buffer_m2 -> device_buffer_m2; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// 9th step: Launch kernel to OpenCL device
	// 9.1. Define work dimension (1, 2 or 3), global_work_size(CUDA grid size) and local_work_size(CUDA threads per block)
	unsigned int work_dim=2;
	//size_t local_work_size[2] ={256, 256};
    size_t local_work_size[2] = {N / 8, M / 8};
	//size_t global_work_size=ceil(N/local_work_size)*local_work_size;
    size_t global_work_size[2] = {N, M};
	printf("\nOpenCL kernel launch!");
	//printf("\nWork dimension: %u | Global work size: %zu | Local work size[1]: %zu | Local work size[2] : %zu | # of Work-groups: %i", work_dim, global_work_size[0], global_work_size[1], local_work_size[0], local_work_size[1], (int)(global_work_size[0]/local_work_size[0]+ global_work_size[1]/local_work_size[1]));
	// 9.2. Launch kernel 
	clock_gettime(CLOCK_MONOTONIC, &t0);
	cl_error=clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clEnqueueNDRangeKernel; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// Blocks until all previously queued OpenCL commands in a command-queue are issued to the associated device and have completed
	clFinish(command_queue);

	// 10th step: Read results from device buffer
	cl_error=clEnqueueReadBuffer(command_queue, device_buffer_m3, CL_TRUE, 0, mat_size_bytes, host_buffer_m3, 0, NULL, NULL);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clEnqueueReadBuffer; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	clFinish(command_queue);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	// DEBUG:
	printf("\n\nDEBUG:");
	printf("\nhost_buffer_m3[0][0]=%i | host_buffer_m3[0][1]=%i | host_buffer_m3[%i][%i]=%i", host_buffer_m3[0 + 0 * N], host_buffer_m3[1 + 0 * N], N-1, M -1,host_buffer_m3[N-1 + (M-1) * M]);

	//
	double t0_ms=(t0.tv_sec*1e3) + ( t0.tv_nsec*1e-6 );
  double t1_ms=(t1.tv_sec*1e3) + ( t1.tv_nsec*1e-6 );
  printf("\n\nElapsed time CLOCK_MONOTONIC (ms): %f", (t1_ms-t0_ms));

	// 11th step: Release OpenCL resources
	clReleaseMemObject(device_buffer_m1);
    clReleaseMemObject(device_buffer_m2);
    clReleaseMemObject(device_buffer_m3);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	// Free host buffers
	free(host_buffer_m1);
    free(host_buffer_m2);
    free(host_buffer_m3);
	free(kernel_string);
	free(platform_name);
	free(device_name);

	printf("\n\n");
	return 0;
} 
