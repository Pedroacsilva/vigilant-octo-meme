/*CGRA 2019_2020
Projecto 1: Sala
Autor: 
Gabriel Lopes
Pedro Silva (2011149228)*/
/*Objectivo: Modelar uma sala com uma porta, lampada, janela, quadro na parede.*/                                               

using namespace std;
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "deecshader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int VAORoom, VBORoomVertices, VBORoomCores, VBORoomVerticesUV, EBORoomIndex, MVPID;
DEECShader * roomShader;

/*Room:
Um paralelipipedo com altura de 3 e comprimento = largura de 6*/

const GLfloat RoomVertices[] = {
	//Room: BASE
	-1.0f, 0.0f, -1.0f,		//Bottom Left
	1.0f, 0.0f, -1.0f,		//Bottom Right	
	1.0f, 0.0f, 1.0f,		//Top Right
	-1.0f, 0.0f, 1.0f,		//Top Left
//-----------------------
	//Room: TOPO
	-1.0f, 1.0f, -1.0f,		//Bottom Left
	1.0f, 1.0f, -1.0f,		//Bottom Right
	1.0f, 1.0f, 1.0f,		//Top Right
	-1.0f, 1.0f, 1.0f,		//Top Left
};

/*Cores: light steel blue  	(176,196,222) https://www.rapidtables.com/web/color/RGB_Color.html
Texturar o chão com uma imagem de azulejo/madeira, texturar paredes com uma porta + janelas.
As texturas serão em função do system clock para determinar se o exterior é dia ou noite.*/

const GLfloat RoomCores[] = {
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f,
	0.69f, 0.77f, 0.87f
};

const unsigned int RoomIndex[] = {
//CHÃO:
	0, 1, 2,
	0, 2, 3,
//PAREDE NORTE:
	3, 2, 6,
	3, 6, 7,
//PAREDE ESTE:
	1, 2, 6,
	1, 6, 5,
//PAREDE OESTE:
	0, 3, 7,
	0, 7, 4,
//PAREDE SUL:
	0, 1, 5,
	0, 5, 4
};


void init(){
	printf("WORK STUDY: Room.\n");
	//Gerar VAO e VBOs
	glGenVertexArrays(1, &VAORoom);
	glGenBuffers(1, &VBORoomVertices);
	glGenBuffers(1, &VBORoomCores);
	//glGenBuffers(1, &VBORoomVerticesUV);

	//Trabalhar no VAO correcto
	glBindVertexArray(VAORoom);

	//Carregar coordenadas dos vértices para VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBORoomVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RoomVertices), RoomVertices, GL_STATIC_DRAW);
	//Definir layout dos dados. Location 0-> Coordenadas
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);

	//Carregar cores dos vértices
	glBindBuffer(GL_ARRAY_BUFFER, VBORoomCores);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RoomCores), RoomCores, GL_STATIC_DRAW);
	//Definir layout dos dados. Location 1-> Cores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(1);

	//Transferir indexação
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORoomIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RoomIndex), RoomIndex, GL_STATIC_DRAW);

	//Carregar e linkar shaders, carregar imagem para textura
	if(roomShader->loadShaders("roomShader.vert","roomShader.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    if(roomShader->linkShaderProgram() == GL_FALSE){
        printf("ERROR LINKING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    MVPID = glGetUniformLocation(roomShader->shaderprogram, "MVPMatrix");
    if(MVPID != -1)
    	printf("MVPID: %i\n", MVPID);
    else
    	printf("MVPID: -1, erro na variável uniforme.\n");
}


void display(){
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	roomShader->startUsing();

	glm::mat4 ModelMatrix = glm::mat4(1.0f);			//Inicializar como matriz identidade
	//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));		//Scale por 2 em todos os eixos
	//Camara na origem a olhar para o ponto (0, 0, 1)
	glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

	glDrawElements(GL_TRIANGLES, 2 * 3 * 6, GL_UNSIGNED_INT, 0);			//Desenhar 2 triangulos / face, desenhar 6 faces
	glFlush();
}


void idle(){
	glutPostRedisplay();
}

void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            roomShader->cleanup();
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

    roomShader = new DEECShader;
    glutSetWindowTitle("Room");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}
