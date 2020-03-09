/*CGRA 2019_2020
Lab 5: Texturas
Autor: Pedro Silva (2011149228)*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#define _gl_h
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include "deecshader.h"
#include "cgraimage.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


GLuint VAOTrianglesID, VBOTrianglesVertex, VBOTrianglesTextureUV, MVPID, timeID, textureID;
unsigned int timeVariable, newTime, oldTime, deltaTime;
bool BWTexture = true;
CGRAimage img1;			//Para load de imagens
//const char* 
DEECShader * myprog;
const GLuint NumVertices = 3 * 2;			//Queremos desenhar 2 triangulos: 6 vertices


const GLfloat vertices [] ={
	-1.0f, 1.0f, 0.0f,			//---------
	-1.0f, -1.0f, 0.0f,			//Primeiro triangulo
	1.0f, 1.0f, 0.0f,			//---------
	-1.0f, -1.0f, 0.0f,			//---------
	1.0f, -1.0f, 0.0f,			//Segundo triangulo
	1.0f, 1.0f, 0.0f			//---------
};
const GLfloat verticesUV[] ={			//Coordenadas texturas
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};		

const unsigned char myTexture[]={			//RGBA
	0, 0, 0, 0,
	255, 255, 255, 0,
	255, 255, 255, 0,
	0, 0, 0, 0
};
void init(){
    glGenVertexArrays(1, &VAOTrianglesID);      //Gerar VAO para triangulos
    glBindVertexArray(VAOTrianglesID);         //Trabalhar no VAO dos triangulos
    glGenBuffers(1, &VBOTrianglesVertex);          //Gerar buffer de dados para coordenadas dos vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBOTrianglesVertex);          //Trabalhar no buffer de coordenadas dos vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);       //Escrever no buffer
    glGenTextures(1, &textureID);				//Gerar estrutura de dados para textura
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_BGR, GL_UNSIGNED_BYTE, myTexture);		//Enviar textura para estrutura de dados
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glEnable(GL_DEPTH_TEST);            //Ligar depth test para fragmentos correctos serem desenhados (e nao simplesmente o mais recente)
    glDepthFunc(GL_LESS);					//Desenhar os fragmentos mais proximos da camara
    if(myprog->loadShaders("textures.vert","textures.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    if(myprog->linkShaderProgram() == GL_FALSE){
        printf("ERROR LINKING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    MVPID = glGetUniformLocation(myprog->shaderprogram, "MVPMatrix");		//Localização das
    timeID = glGetUniformLocation(myprog->shaderprogram, "time");			//variaveis uniformes
    glVertexAttribPointer(
                            0,          //localizacao do atributo. Deve ser concordante com layout do shader
                            3,          //tamanho dos dados
                            GL_FLOAT,   //tipo dos dados
                            GL_FALSE,   //normalizado?
                            0,          //stride
                            BUFFER_OFFSET(0)    //offset
                            );
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &VBOTrianglesTextureUV);			//Gerar VBO para texturas
    glBindBuffer(GL_ARRAY_BUFFER, VBOTrianglesTextureUV);		//Trabalhar neste VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesUV), verticesUV, GL_STATIC_DRAW);		//Encher este VBO
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));			//Dizer como os dados estao estruturados aos shaders
    glEnableVertexAttribArray(1);
    for(int i = 0; i < NumVertices * 3; i = i + 3)
        printf("Vertice %i: (%f, %f, %f)\n", i , vertices[i], vertices[i+1], vertices[i+2]);
    for(int i = 0; i < NumVertices * 2; i = i + 3)
        printf("Vertice UV %i: (%f, %f)\n", i , vertices[i], vertices[i+1]);
    printf("MVPMatrix: %i\n", MVPID);
    if(glIsBuffer(VBOTrianglesVertex) == GL_TRUE)
        printf("VBOTrianglesVertex é um buffer.\n");
    else
        printf("VBOTrianglesVertex não é um buffer.\n");
    if(!img1.loadPPM("img1.ppm"))
    	printf("Error loading image.\n");
    else
    	printf("Image loaded successfully.\n");
}




void display(){
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);     //cor de fundo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//Limpar buffer de cores + depth
    myprog->startUsing();
    glm::mat4 ModelMatrix = glm::mat4(1.0f);	//inicializar matriz modelo como identidade
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, -10.0f));		//Translação para (0, 0, -10)
    glm::mat4 ViewMatrix       = glm::lookAt(
                                glm::vec3(0, 0, 0), // Camera na origem (coordenadas mundo)
                                glm::vec3(0, 0, -10),  //observa o ponto (em 0, 0, -10)
                                glm::vec3(0, 1, 0)  // camara de "pé"
                                );
    glm::mat4 ProjectionMatrix = glm::perspective(
                                glm::radians(45.0f),        //Field of View
                                4.0f / 3.0f,                //Aspect Ratio
                                0.1f,                       //nearplane
                                100.0f                       //farplane
                                );
    glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniform1i(timeID, timeVariable);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}


void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            myprog->cleanup();
            exit(EXIT_SUCCESS);
            break;
        case 't':
        	printf("\nTexture toggled.\n");
        	BWTexture = !BWTexture;
        	if(!BWTexture){
        		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1.width, img1.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img1.data);		//Enviar textura para estrutura de dados
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			else{
			    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_BGR, GL_UNSIGNED_BYTE, myTexture);		//Enviar textura para estrutura de dados
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			break;
    }
}


void idle(void){
	deltaTime = newTime - oldTime;
    timeVariable += deltaTime;
    oldTime = newTime;
    newTime = glutGet(GLUT_ELAPSED_TIME);
    printf("\rTime: %i ms.", timeVariable);
	glutPostRedisplay();
}




int main(int argc, char** argv) {
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_RGBA|GLUT_3_2_CORE_PROFILE);
#else // LINUX
    glutInitDisplayMode(GLUT_RGBA);
#endif
    glutInitWindowSize(512, 512);

#ifndef __APPLE__
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#endif
    glutCreateWindow(argv[0]);
#ifndef __APPLE__
    if (glewInit()) {
    cerr << "Unable to initialize GLEW ... exiting" << endl; exit(EXIT_FAILURE);
    }
#endif

    myprog = new DEECShader;
    glutKeyboardFunc( keyPressed );
    glutIdleFunc( idle );       //Definir callback function para no events (idle)
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}







