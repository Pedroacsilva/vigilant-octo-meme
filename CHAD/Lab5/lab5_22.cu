//nvcc -o lab5_22 lab5_22.cu
/*Author:Pedro Silva*/
/*2. Implemente um programa em CUDA que calcule a soma de todos os elementos de um vetor de
tamanho N. Teste para vários valores de N.*/
//2.2. Implemente uma nova versão otimizada baseada em memória partilhada.
//Assume each block has 64 threads (cheating but idc)
#define BLOCK_SIZE 64
__global__ void vectorsum2_2(int * device_buffer, int N){
	//THREAD ID
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	//Declarar shared memory, BLOCK_SIZE integers
	__shared__ int elements[BLOCK_SIZE];
	/*We have N elements to sum. We'll break the sum of all elements as sums of one element + the element half a block away. For each iteration
	of the sum, halve the number of threads per block. Repeat until we have one thread per block.*/
	/*Each thread loads one element from global to shared memory.*/
	if(index <= N)
		elements[index] = device_buffer[index] ;
	//syncthreads to make sure all elements are loaded into shared memory
	__syncthreads();
	/*For N elements to sum, N/2 threads sum 2 elements together*/
	int threads_per_block = BLOCK_SIZE/2;
	/*Loop where each threads sums 2 elements in shared memory and stores it in shared mem. Halve the number of threads per block
	each iteration, until we have one thread (WARNING: Need to define workflow when number of threads is odd.)*/
	while(threads_per_block > 1 && threadIdx.x <= threads_per_block){
		elements[threadIdx.x] = elements[threadIdx.x] + elements[threadIdx.x + threads_per_block];
		threads_per_block /= 2;
		__syncthreads();	//Make sure each thread has finished computation before doing next iteration
	}
	/*One thread per block, load sum from shared memory to global device memory. Host will then load  and sum those values*/
	if(threadIdx.x == 0)
		device_buffer[index] = elements[0];
}		

int main(){
	//32768 = 2¹⁵;
	for(int N = BLOCK_SIZE; N <= BLOCK_SIZE * 8; N = N*2){
		int *device_buffer = NULL;
		int err = cudaMalloc(&device_buffer, sizeof(int) * N);
		if(err != cudaSuccess){
			fprintf(stderr, "Error allocating memory on device.\n");
			return(-1);
		}
		vectorsum2_2<<< N/BLOCK_SIZE + 1, BLOCK_SIZE >>>(device_buffer, N);
		int results[N / BLOCK_SIZE];
		//Fetch sum results from device to host
		for(int i = 0; i < N/BLOCK_SIZE; i++)
			cudaMemcpy(results[i], device_buffer[i * BLOCK_SIZE], sizeof(int));
		printf("Sum of a vector of %i elements: %i.\n", N, result);
		//FREE DEVICE MEMORY
		cudaFree(device_buffer);
	}
	
	return 0;
}


