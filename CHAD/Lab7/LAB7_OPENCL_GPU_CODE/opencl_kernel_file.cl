__kernel void device_func_name(__global int *device_buffer, int N)
{
	// Work-item identifier (1 dimensional problem)
	int index=get_global_id(0);
	// CODE
	if(index<N)
		device_buffer[index]=index*2;
}