/*Author:
Pedro Silva
*/

// Analise o seguinte kernel, ​ colorToGreyScaleConversion()​ , que produz a conversão de cor
// para escala de cinzentos de uma imagem com mapeamento de dados em threads 2D
// Device code
// We have 3 channels corresponding to RGB
// The input image is encoded as unsigned characters [0, 255]
__global__ void colorToGreyScaleConvertion(unsigned char * grayImage, unsigned char
	*rgbImage, int width, int height)
{
	int Col=threadIdx.x+(blockIdx.x*blockDim.x);
	int Row=threadIdx.y+(blockIdx.y*blockDim.y);
	if (Col<width && Row<height){
// get 1D coordinate for the grayscale image
		int greyOffset=Row*width + Col;
// one can think of the RGB image having
// CHANNEL times columns of the gray scale image
		int rgbOffset=greyOffset*CHANNELS;
		unsigned char r=rgbImage[rgbOffset];
// red value for pixel
		unsigned char g=rgbImage[rgbOffset+1];
// green value for pixel
		unsigned char b=rgbImage[rgbOffset+2];
// blue value for pixel
// perform the rescaling and store it
// We multiply by floating point constants
		grayImage[grayOffset]=0.21f*r + 0.71f*g + 0.07f*b;
	}
}