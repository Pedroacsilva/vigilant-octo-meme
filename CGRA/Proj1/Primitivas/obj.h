#ifndef _MYOBJ_H
#define _MYOBJ_H
#include "deecshader.h"
#include "cgraimage.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class genericModel{
protected:
    GLuint vao, texID;            //VAO & Texture IDs
    GLfloat *  vCoords, *vColors;           //Ponteiros para matrizes de dados
    GLfloat * vTexCoords;                   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    GLuint * vEBO;
    CGRAimage textureImage;           //Ponteiro para imagem de textura
    int NumVertices;
    unsigned int vboCoords, vboColors, vboTexCoords, vboEBO;    //IDs para VBOs
    
public:

    //genericModel();
    virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
       shaderProg->startUsing();
	// bind texture

	// apply model transformations

       glDrawArrays(GL_TRIANGLES, 0, NumVertices);
       shaderProg->stopUsing();
   }
   void setImageTexture(char * textureName){
        glBindTexture(GL_TEXTURE_2D, texID);
        textureImage.loadPPM(textureName);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
};


class Square: public genericModel{
public:
    //Constructor
    Square();
    //Destructor
    ~Square();
    virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix);
};

class Cube: public genericModel{

public:
    //Constructor & Destructor
    Cube();
    ~Cube();
    virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix);
    //virtual void setImageTexture(char * textureName);
};
#endif



