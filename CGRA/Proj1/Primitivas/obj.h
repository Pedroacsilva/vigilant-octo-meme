#ifndef _MYOBJ_H
#define _MYOBJ_H
#include "deecshader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class genericModel{
protected:
    unsigned int vao, vboPositions, vboColors, vboTex;
    int NumVertices;
    
public:

    genericModel();
    virtual void drawShape(DEECShader * shaderProg, glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){
       shaderProg->startUsing();
	// bind texture

	// apply model transformations

       glDrawArrays(GL_TRIANGLES, 0,NumVertices);
       shaderProg->stopUsing();
   }
};


class Square: public genericModel{
public:
    void drawShape(DEECShader * shaderProg, glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
    //Constructor
    Square();
};

/*class Cylinder: public genericModel{

public:
    Cylinder(float radius, float height);
    //drawShape(GLuint ShaderProg); // uncomment if needed
};
*/
#endif



