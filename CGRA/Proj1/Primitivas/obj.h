#ifndef _MYOBJ_H
#define _MYOBJ_H
#include "deecshader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class genericModel{
protected:
    GLuint vao;            //VAO
    GLfloat *  vCoords, *vColors;           //Ponteiros para matrizes de dados
    GLfloat * vTexCoords;                   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    GLuint * vEBO;
    int NumVertices;
    
public:

    //genericModel();
    virtual void drawShape(DEECShader * shaderProg){
       shaderProg->startUsing();
	// bind texture

	// apply model transformations

       glDrawArrays(GL_TRIANGLES, 0, NumVertices);
       shaderProg->stopUsing();
   }
};


class Square: public genericModel{
public:
    //Constructor
    Square();
    virtual void drawShape(DEECShader * shaderProg);
    //GLfloat *getvCoords();
};

/*class Cylinder: public genericModel{

public:
    Cylinder(float radius, float height);
    //drawShape(GLuint ShaderProg); // uncomment if needed
};
*/
#endif



