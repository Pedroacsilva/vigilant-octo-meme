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
    GLuint vao, texID;                      //VAO & Texture IDs
    GLfloat *  vCoords, *vColors;           //Ponteiros para matrizes de dados
    GLfloat * vTexCoords;                   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    GLuint * vEBO;
    CGRAimage textureImage;                 //Ponteiro para imagem de textura
    int NumVertices;
    glm::mat4 MVPMatrix;
    unsigned int vboCoords, vboColors, vboTexCoords, vboEBO;    //IDs para VBOs
    
public:

    //genericModel();
    virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
       shaderProg->startUsing();
       glDrawArrays(GL_TRIANGLES, 0, NumVertices);
       shaderProg->stopUsing();
   }
   //Setters
   void setVertexColor(GLfloat R, GLfloat G, GLfloat B){
    for(int i = 0; i < NumVertices; i ++){
        vColors[i * 3] = R; vColors[i * 3 + 1] = G; vColors[i * 3 + 2] = B;
    }
    //Enviar dados para device
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NumVertices, vColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);
   }
   //--------------------------------------
   void setImageTexture(char * textureName){
        glBindTexture(GL_TEXTURE_2D, texID);
        textureImage.loadPPM(textureName);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    //--------------------------------------
    void setMVPMatrix(glm::mat4 MVPMatrix){
        this->MVPMatrix = MVPMatrix;
    }
};


class Square: public genericModel{
public:
    //Constructor
    Square();
    //Destructor
    ~Square();
    void drawShape(DEECShader * shaderProg);
};

class Cube: public genericModel{

public:
    //Constructor & Destructor
    Cube();
    ~Cube();
    //virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix);
    void drawShape(DEECShader * shaderProg);
    //virtual void setImageTexture(char * textureName);
};


class Cylinder: public genericModel{
public:
    //Constructor & Destructor
    Cylinder();
    ~Cylinder();
    Cylinder(float upperRadius, float lowerRadius, float height);
    //virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix);
    void drawShape(DEECShader * shaderProg);
};

class Sphere: public genericModel{
public:
    //Constructor & Destructor
    Sphere();
    ~Sphere();
  //  virtual void drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix);
    void drawShape(DEECShader * shaderProg);
};


const unsigned char chessBoardTexture[]={           //RGBA
    0, 0, 0, 0,
    255, 255, 255, 0,
    255, 255, 255, 0,
    0, 0, 0, 0
};
#endif



