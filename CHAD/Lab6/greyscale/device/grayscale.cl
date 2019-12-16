//CHAD 2019/2020
//Autor: Pedro Silva
//Kernel opencl que dado uma imagem de dimensoes N*M, com 3 canais de cor (RGB), computa a imagem em tons de cinza
//Cada thread computará o tom de cinza duma imagem de acordo com o seu global index (Row, Col)

__kernel void greyscale(__global const unsigned char* d_image_rgb, __global const int * d_image_gs, __global const int width, __global const int height){
    //get global index
    int col = get_global_id(0);
    int row = get_global_id(1);
    int index = row * M + col;
    //Garantir que estamos no scope do problema
    if(row <  N && col < M){
        gs_offset = row * width + col;
        rgb_index = gs_offset * 4;
        unsigned char r = d_image_rgb[rgb_index];
        unsigned char g = d_image_rgb[rgb_index + 1];
        unsigned char b = d_image_rgb[rgb_index + 2];
        d_image_gs[gs_offset] =  0.21 * r + 0.71 * g + 0.07 * b;
    }
}
