// gcc -I /usr/local/cuda-10.1/include/ host_code_file.c -lm -lOpenCL -o opencl_CHAD_example.o
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
	unsigned int N=65536;
	size_t vec_size_bytes=N*sizeof(int);

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
	// 5.1. Read opencl_kernel_file.cl to a string
	size_t file_size=0;
	char *kernel_string=NULL;
	FILE *fp=fopen("opencl_kernel_file.cl", "rb");
	if(fp==NULL)
	{
		printf("\nERROR: fopen(%s)", "opencl_kernel_file.cl");
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
	kernel=clCreateKernel(program, "device_func_name", &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateKernel; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}

	// 8th step: Assign the kernel arguments
	// 8.1. Define host buffers
	int *host_buffer=(int *)malloc(vec_size_bytes);
	cl_mem device_buffer=clCreateBuffer(context, CL_MEM_WRITE_ONLY, vec_size_bytes, NULL, &cl_error);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clCreateBuffer device_buffer; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// 8.4. Set arguments to the correspondent kernel
	cl_error=clSetKernelArg(kernel, 0, sizeof(cl_mem), &device_buffer);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg device_buffer; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	cl_error=clSetKernelArg(kernel, 1, sizeof(int), &N);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clSetKernelArg N; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// 9th step: Launch kernel to OpenCL device
	// 9.1. Define work dimension (1, 2 or 3), global_work_size(CUDA grid size) and local_work_size(CUDA threads per block)
	unsigned int work_dim=1;
	size_t local_work_size=256;
	size_t global_work_size=ceil(N/local_work_size)*local_work_size;
	printf("\nOpenCL kernel launch!");
	printf("\nWork dimension: %u | Global work size: %zu | Local work size: %zu | # of Work-groups: %i", work_dim, global_work_size, local_work_size, (int)(global_work_size/local_work_size));
	// 9.2. Launch kernel 
	clock_gettime(CLOCK_MONOTONIC, &t0);
	cl_error=clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
	if(cl_error!=CL_SUCCESS)
	{
		printf("\nERROR: clEnqueueNDRangeKernel; ERROR code:%i", cl_error);
		printf("\n\n");
		return 0;
	}
	// Blocks until all previously queued OpenCL commands in a command-queue are issued to the associated device and have completed
	clFinish(command_queue);

	// 10th step: Read results from device buffer
	cl_error=clEnqueueReadBuffer(command_queue, device_buffer, CL_TRUE, 0, vec_size_bytes, host_buffer, 0, NULL, NULL);
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
	printf("\nhost_buffer[0]=%i | host_buffer[1]=%i | host_buffer[%i]=%i", host_buffer[0], host_buffer[1], N-1, host_buffer[N-1]);

	//
	double t0_ms=(t0.tv_sec*1e3) + ( t0.tv_nsec*1e-6 );
  double t1_ms=(t1.tv_sec*1e3) + ( t1.tv_nsec*1e-6 );
  printf("\n\nElapsed time CLOCK_MONOTONIC (ms): %f", (t1_ms-t0_ms));

	// 11th step: Release OpenCL resources
	clReleaseMemObject(device_buffer);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	// Free host buffers
	free(host_buffer);
	free(kernel_string);
	free(platform_name);
	free(device_name);

	printf("\n\n");
	return 0;
}