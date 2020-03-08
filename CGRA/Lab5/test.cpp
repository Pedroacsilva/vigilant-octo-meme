#include "cgraimage.h"
#include <iostream>
using namespace std;

int main(){
    CGRAimage image1;
    const char * img1="img1.ppm";
    const char * img2="img2.bmp";
    
    if(!image1.loadPPM(img1))
	cout << "could not open image file "<< img1<<endl;
    else
	cout <<"Image file "<<img1<<" -> " <<image1.width<<"x"<<image1.height<<"\n";
   
    
    if(!image1.loadBMP(img2))
	cout << "could not open image file "<<endl;
    else
	cout <<"Image file "<< img2 <<" -> "<<image1.width<<"x"<<image1.height<<"\n";
 
}
