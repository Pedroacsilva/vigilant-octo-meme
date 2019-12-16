//Kernel que executa saxpy simples
__kernel void saxpy(__global const int A, __global const int * X, __global int * Y, __global int * Z) {
 
    //Obter index e fazer saxpy apropriado
    int i = get_global_id(0);
    Z[i] = A * X[i] + Y[i];
}