/*CGRA 2019_2020
Projecto 1: Landascape
Autor: 
Gabriel Lopes
Pedro Silva (2011149228)*/
/*Objectivo: Modelar uma paisagem constituida por um campo verde e um céu azul claro ou escuro, de acordo com tempo do dia*/

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

unsigned int VAOLand, VAOSky, VBOLandVertices, VBOLandColors, EBOLandIndex, VBOSkyVertices, VBOSkyTexCoords, EVOSkyIndex;
unsigned int MVPID;
DEECShader * landShader;

const GLfloat LandVertices[] = {
	-1.0f, 0.0f, -1.0f,			//bottom left
	1.0f, 0.0f, -1.0f,			//bottom right
	1.0f, 0.0f, 1.0f,			//top right
	-1.0f, 0.0f, 1.0f
};

//RGB: (124, 252, 0) Lawn green https://www.rapidtables.com/web/color/RGB_Color.html
const GLfloat LandVerticesColors[] = {
	0.49f, 0.99f, 0.0f,
	0.49f, 0.99f, 0.0f,
	0.49f, 0.99f, 0.0f,
	0.49f, 0.99f, 0.0f
};

const unsigned int LandVerticesIndex[] ={
	0, 1, 2,
	0, 2, 1
};

void init(){
	printf("WORK STUDY: Landscape");
	glGenVertexArrays(1, &VAOLand);
	glGenBuffers(1, &VBOLandVertices);
	glGenBuffers(1, &VBOLandColors);
	glGenBuffers(1, &EBOLandIndex);
	glBindVertexArray(VAOLand);


	//Carregar coordenadas
	glBindBuffer(GL_ARRAY_BUFFER, VBOLandVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LandVertices), LandVertices, GL_STATIC_DRAW);

	//Definir layout dos dados para shaders. Location 0-> coordenadas vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);

	//Carregar cores
	glBindBuffer(GL_ARRAY_BUFFER, VBOLandColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LandVerticesColors), LandVerticesColors, GL_STATIC_DRAW);

	//Location 1 -> Cores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(1);

	//Transferir indexação das coordenadas vértices usando o respectivo EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOLandIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LandVerticesIndex), LandVerticesIndex, GL_STATIC_DRAW);

	if(landShader->loadShaders("tableShader.vert","tableShader.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        //exit(EXIT_FAILURE);
    }
    if(landShader->linkShaderProgram() == GL_FALSE){
        printf("ERROR LINKING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
	MVPID = glGetUniformLocation(landShader->shaderprogram, "MVPMatrix");
    if(MVPID != -1)
    	printf("MVPID: %i\n", MVPID);
    else
    	printf("MVPID: -1, erro na variável uniforme.\n");
}



void display(){
	//Limpar display com um azul escuro
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	landShader->startUsing();

	glm::mat4 ModelMatrix = glm::mat4(1.0f);            //inicializar matriz modelo
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20.0f));		//Scaling em todos os eixos por 20
	glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));


	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
/*void glDrawElements( 	GLenum mode,	->Desenhar triangulos
					  	GLsizei count,	->Desenhar 6 vertices
  						GLenum type,	->Indexação definida por unsigned ints
					  	const void * indices	-> estrutura de dados com indices, ou quando usamos EBO, o seu offset
					  	);*/
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFlush();	
}


void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            landShader->cleanup();
            exit(EXIT_SUCCESS);
            break;
	}
}

void idle(){
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

    landShader = new DEECShader;
    glutSetWindowTitle("Landscape");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}

