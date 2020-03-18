/*CGRA 2019_2020
Projecto 1: Chessboard
Autor: 
Gabriel Lopes
Pedro Silva (2011149228)*/
/*Objectivo: Modelar um tabuleiro de xadrez, um quadrado com textura axadrezada*/

using namespace std;
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "deecshader.h"

unsigned int EBOVertices, VBOVertices, VAOBoard, VBOVerticesUV, textureID;			//EBO->Element Buffer Object, contem informacao a ser indexada
DEECShader * simpleShader;
//Posicao dos 4 vertices do quadrado, para serem indexados

const GLfloat vertices[] = {
	0.9f, 0.9f, 0.0f,		//Top Right
	0.9f, -0.9f, 0.0f, 		//Bottom right
	-0.9f, -0.9f, 0.0f,		//Bottom Left
	-0.9f, 0.9f, 0.0f		//Top Left
};

const unsigned int verticesIndex[] = {
	1, 0, 3,			//FIRST TRIANGLE
	2, 1, 3				//SECOND TRIANGLE
};


//coordenadas textura dos vértices, a ser indexadas
const GLfloat verticesUV[] = {
	4.0f, 4.0f,			//Top Right
	4.0f, 0.0f,			//Bottom Right
	0.0f, 0.0f,			//Bottom Left
	0.0f, 4.0f,			//Top Left
};

const unsigned char chessboardTexture[] = {			//Textura 2x2. Um tabuleiro de xadrez tem dimensoes 8x8, pelo que vamos propagar esta textura 4 vezes em U e em V
	255, 255, 255, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	255, 255, 255, 0
};

void init(){
	printf("WORK STUDY: Chessboard.\n");
	//Gerar objectos para conter os dados
	glGenVertexArrays(1, &VAOBoard);
	glGenBuffers(1, &VBOVertices);
	glGenBuffers(1, &VBOVerticesUV);
	glGenBuffers(1, &EBOVertices);
	//Trabalhar no VAO criado
	glBindVertexArray(VAOBoard);

	//Transferir coordenadas dos vértices para respectivo VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//STATIC DRAW-> Dados não mudam durante a execução

	//Definir layout dos dados para shaders. Location 0-> coordenadas vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);


	//Transferir coordenadas texturas dos vértices para respectivo VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOVerticesUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesUV), verticesUV, GL_STATIC_DRAW);


	//Location 1 -> Coordenadas textura vertices
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(1);


	//Carregar textura
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessboardTexture);		//Enviar textura para estrutura de dados
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Transferir indexação das coordenadas vértices usando o respectivo EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVertices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);





	/*//Transferir indexação das coordenadas textura dos vértices		(Indexação igual à das coordenadas)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVerticesUV);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);*/


	//Carregar e linkar shaders, carregar imagem para textura
	if(simpleShader->loadShaders("chessboard.vert","chessboard.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    if(simpleShader->linkShaderProgram() == GL_FALSE){
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
	simpleShader->startUsing();


	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
/*void glDrawElements( 	GLenum mode,	->Desenhar triangulos
					  	GLsizei count,	->Desenhar 6 vertices
  						GLenum type,	->Indexação definida por unsigned ints
					  	const void * indices	-> estrutura de dados com indices, ou quando usamos EBO, o seu offset
					  	);*/


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFlush();	
}

void idle(){
	glutPostRedisplay();
}


void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            simpleShader->cleanup();
            exit(EXIT_SUCCESS);
            break;
	}
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

    simpleShader = new DEECShader;
    glutSetWindowTitle("Chessboard");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}
