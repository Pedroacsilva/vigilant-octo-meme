 
__kernel void vectorSum_2(__global int *device_buffer_v1, __global int *device_buffer_v2, __global int *device_buffer_v3, int N)
{
	// Work-item identifier (1 dimensional problem)
	int index=get_global_id(0);
	// CODE
	if(index<N){
        device_buffer_v1[index] = 2;
        device_buffer_v2[index] = 3;
		device_buffer_v3[index]= device_buffer_v1[index] + device_buffer_v2[index];
    }
    if(index == 0)
        printf("Work item 0: d_1 = %i d_2 = %i d_3 = %i.\n", device_buffer_v1[index], device_buffer_v2[index], device_buffer_v3[index]);
}
