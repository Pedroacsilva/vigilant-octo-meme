/*function to read a .ppm file in c */
/*
Author: Pedro Silva
*/
#include"readppm.h"

void readppm(const char* filename){
	char buffer[16];			//Pointer to a 16 byte long buffer

	fp = fopen(filename, "rb");	//open file in binary mode
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'.\n", filename);
		return(-1);				//exit with error
	}
	//read image format (should be "P6", we want a binary valued RGB image (P6))
	//So we try to read 16 characters, we should get the first line of the ppm file (PX, where 1 <= X <= 6)
	if(fgets(buffer, sizeof(buffer), fp) == NULL){
		fprintf(stderr,"Error reading file '%s'.\n", filename);
		fclose(fp);
		return(-1);
	}
	//Check the image format
	if(buffer[0] != 'P' ||   buffer[1] != '6'){
		fprintf(stderr, "File isn't .ppm format '%s'.\n", filename);
		fclose(fp);
	}