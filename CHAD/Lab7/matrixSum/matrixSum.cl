 
__kernel void matrixSum(__global int *device_buffer_v1, __global int *device_buffer_v2, __global int *device_buffer_v3, int N, int M)
{
	// Work-item identifier (1 dimensional problem)
    int col = get_global_id(0);
    int row = get_global_id(1);
    int index = col + N * row;
	// CODE
	if(col<N && row < M)
		device_buffer_v3[index]= device_buffer_v1[index] + device_buffer_v2[index];
    if(index == 0)
        printf("Work item 0: d_1 = %i d_2 = %i d_3 = %i.\n", device_buffer_v1[index], device_buffer_v2[index], device_buffer_v3[index]);
}
