//nvcc -o lab5_2 lab5_2.cu
/*Author:
Pedro Silva
*/
/*2. Implemente um programa em CUDA que calcule a soma de todos os elementos de um vetor de
tamanho N. Teste para vários valores de N.*/
/*2.1. Implemente uma versão simples (sem recorrer a optimizações).*/

__global__ void vectorsum2_1(int * device_buffer){
	//THREAD ID
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	/*Temos N elementos no vector. Vamos partir a soma de todos os elementos como a soma de um elemento com o elemento à sua direita.
	Repetir até so termos um elemento (a cada iteração, temos metade dos elementos).*/
	//Assumir que só lançamos um bloco de threads (blockIdx.x = 1 para todas as threads.)
	int num_of_threads = blockDim.x;			//Se só lançamos um bloco...
	int distance = 1;			//Distancia entre elemtnos a somar

	/*Este ciclo executa enquanto tivermos mais que uma thread e so se a thread ainda estiver no "scope" da soma.*/
	while(num_of_threads > 0 && index < num_of_threads){
		int primeiro = index * distance * 2;		//na primeira iteracao: 1a thread, index 0, 2a thread, index 2, 3a thread, index 4
		int segundo = primeiro + distance;			//na primeira iteracao: 1a thread, index 1, 2a thread, index 3, 3a thread, index 5
		device_buffer[primeiro] = device_buffer[primeiro] + device_buffer[segundo];

		//passou uma iteracao: duplicar a distancia entre elementos a somar e dividir por 2 o numero de threads activas
		distance = distance * 2;
		num_of_threads = num_of_threads / 2;

	}
}		

int main(){
	//32768 = 2¹⁵;
	for(int N = 8; N <= 32768; N = N*2){
		int *device_buffer = NULL;
		int err = cudaMalloc(&device_buffer, sizeof(int) * N);
		if(err != cudaSuccess){
			fprintf(stderr, "Error allocating memory on device.\n");
			return(-1);
		}
		vectorsum2_1<<< 1, N/2 >>>(device_buffer);
		int result;
		/*Vamos buscar o resultado da soma ao primeiro elemento do device_buffer*/
		cudaMemcpy(&result, device_buffer, sizeof(int));
		printf("Resultado da soma de um vector de %i elementos: %i.\n", N, result);
	}
	
	return 0;
}


