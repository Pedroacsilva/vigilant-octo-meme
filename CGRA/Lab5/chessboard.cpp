/*CGRA 2019_2020
Projecto 1: Chessboard
Autor: Pedro Silva (2011149228)*/
/*Objectivo: Modelar um tabuleiro de xadrez, um quadrado com textura axadrezada*/

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
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "deecshader.h"
#include "cgraimage.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



const unsigned int EBOVertices, VBOBoard, VAOVertices, VBOVerticesUV, EBOVerticesUV;			//EBO->Element Buffer Object, contem informacao a ser indexada
DEECShader * shadersProg;			//Programa que implementa vertex + fragment shaders
//CGRAimage  chessboardTexture;


//Posicao dos 4 vertices do quadrado, para serem indexados

const GLfloat vertices[] = {
	0.5f, 0.5f, 0.0f,		//Top Right
	0.5f, -0.5f, 0.0f, 		//Bottom right
	-0.5f, -0.5f, 0.0f,		//Bottom Left
	-0.5f, 0.5f, 0.0f		//Top Left
};

const unsigned int verticesIndex[] = {
	0, 1, 3,			
	1, 2, 3
};


//coordenadas textura dos vértices, a ser indexadas
const unsigned int verticesUV[] = {
	4.0f, 4.0f,			//Top Right
	4.0f, 0.0f,			//Bottom Right
	0.0f, 0.0f,			//Bottom Left
	0.0f, 4.0f,			//Top Left
};

const unsigned char chessboardTexture[] = {			//Textura 2x2. Um tabuleiro de xadrez tem dimensoes 8x8, pelo que vamos propagar esta textura 4 vezes em U e em V
	255, 255, 255, 0,								//WWWWW BBBBB
	0, 0, 0, 0,										//BBBBB WWWWW
	255, 255, 255, 0,
	0, 0, 0, 0
};

void init(){
	printf("WORK STUDY: Chessboard.\n");
	//Gerar objectos para conter os dados
	glGenVertexArrays(1, &VAOBoard);
	glGenBuffers(1, &VBOBoard);
	glGenBuffers(1, &EBOBoard);
	//Trabalhar no VAO criado
	glBindVertexArray(VAOBoard);

	//Transferir coordenadas dos vértices para respectivo VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//STATIC DRAW-> Dados não mudam durante a execução

	//Transferir indexação das coordenadas vértices usando o respectivo EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVertices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);

	//Definir layout dos dados para shaders. Location 0->vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0, (void*) 0);
	

	//Transferir coordenadas texturas dos vértices para respectivo VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOVerticesUB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesUV), verticesUV, GL_STATIC_DRAW);

	/*//Transferir indexação das coordenadas textura dos vértices		(Indexação igual à das coordenadas)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVerticesUV);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);*/


	//Carregar e linkar shaders, carregar imagem para textura
	if(shadersProg->loadShaders("chessboard.vert","chessboard.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    if(shadersProg->linkShaderProgram() == GL_FALSE){
        printf("ERROR LINKING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    /*if(!chessboardTexture.loadPPM("chessboard.ppm"))
        printf("Error loading image.\n");
    else
        printf("Image loaded successfully.\n");*/



}


void display(){
	//Limpar display com um azul escuro
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
/*void glDrawElements( 	GLenum mode,	->Desenhar triangulos
					  	GLsizei count,	->Desenhar 6 vertices
  						GLenum type,	->Indexação definida por unsigned ints
					  	const void * indices	-> estrutura de dados com indices, ou quando usamos EBO, o seu offset
					  	);*/


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	
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
    glutSetWindowTitle("Chessboard");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
