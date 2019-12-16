/*CHAD 2019/2020
Autor: Pedro Silva
Kernel que multiplica duas matrizes quadradas, dimensoes 1024*1024*/
__kernel void matMul(__global const int * X, __global const int * Y, __global int * restrict * Z){
	//buscar indices de thread
	int col = get_global_id(0);
	int row = get_global_id(1);
	int result = 0;
	//garantir que estamos no scope do problema
	if(col < 1024 && row < 1024){
		for(int i = 0; i < 1024; i++)
			result += X[row * 1024 + i] * Y[col + i * 1024];
	}
	//Escerever no buffer de saida, Z
	Z[row * 1024 +  col] = result;
}