/*CGRA 2019_2020
Projecto 1: Sala
Autor: 
Gabriel Lopes
Pedro Silva (2011149228)*/
/*Objectivo: Modelar uma sala, com uma mesa no meio, um tabuleiro de xadrez em cima da mesa,
uma porta, uma janela  e uma fonte de luz no tecto.*/

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

unsigned int VAOMesa, VBOMesaVertices, VBOMesaCores, EBOMesaIndex, MVPID;



/*MESA:
1 paralelipipedo (alto e magro) para perna da mesa, outro paraleleipipedo (baixo e gordo) para tampo da mesa.*/


const unsigned int MesaNumVertices = 16;		//Mesa é composta por 2 paralelipipedos, 8 vertices cada-> 16 vertices no total
DEECShader * tableShader;


const GLfloat MesaVertices[] ={
	//PERNA: BASE
	-0.2f, 0.0f, -0.2f,
	0.2f, 0.0f, -0.2f,
	0.2f, 0.0f, 0.2f,
	-0.2f, 0.0f, 0.2f,
	//PERNA: TOPO
	-0.2f, 0.5f, -0.2f,
	0.2f, 0.5f, -0.2f,
	0.2f, 0.5f, 0.2f,
	-0.2f, 0.5f, 0.2f,
	//TAMPO: BASE
	-0.8f, 0.5f, -0.8f,
	0.8f, 0.5f, -0.8f,
	0.8f, 0.5f, 0.8f,
	-0.8f, 0.5f, 0.8f,
	//TAMPO: TOPO
	-0.8f, 0.6f, -0.8f,
	0.8f, 0.6f, -0.8f,
	0.8f, 0.6f, 0.8f,
	-0.8f, 0.6f, 0.8f,
};	


//Queremos uma mesa castanha, pelo que todos os vértices da perna têm a mesma cor, e o tampo uma tonalidade de castanhao ligeiramente diferente

const GLfloat MesaCores[]={
	//8 vértices da perna: Saddle Brown, ver https://www.rapidtables.com/web/color/RGB_Color.html
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	0.55f, 0.27f, 0.7f,
	//8 vértices do tampo: Sienna
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f,
	0.63f, 0.32f, 0.18f
};


const unsigned int MesaIndex[] ={
//PERNA:
	1, 5, 0,
	0, 5, 4,
//-------------------------------------
	1, 2, 6,
	1, 6, 5,
//-------------------------------------	
	2, 3, 7,
	2, 6, 7,
//-------------------------------------
	3, 0, 4,
	3, 7, 4,
//TOPO:
	9, 13, 8,
	8, 13, 12,
//-------------------------------------
	9, 10, 14,
	9, 14, 13,
//-------------------------------------
	10, 11, 15,
	10, 14, 15,
//-------------------------------------
	11, 8, 12,
	11, 15, 12
};

void init(){
	printf("WORK STUDY: Table.\n");
	//Gerar VAO e VBOs
	glGenVertexArrays(1, &VAOMesa);
	glGenBuffers(1, &VBOMesaVertices);
	glGenBuffers(1, &VBOMesaCores);
	glGenBuffers(1, &EBOMesaIndex);
	//Trabalhar no VAO criado
	glBindVertexArray(VAOMesa);

	//Transferir coordenadas dos vértices para respectivo VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOMesaVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MesaVertices), MesaVertices, GL_STATIC_DRAW);

	//Definir layout dos dados para shaders. Location 0-> coordenadas vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);

	//Transferir cores dos vértices para VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOMesaCores);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MesaCores), MesaCores, GL_STATIC_DRAW);

	//Location 1-> cores vértices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	//Transferir indexação para EBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOMesaIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MesaIndex), MesaIndex, GL_STATIC_DRAW);

//Carregar e linkar shaders, carregar imagem para textura
	if(tableShader->loadShaders("tableShader.vert","tableShader.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    if(tableShader->linkShaderProgram() == GL_FALSE){
        printf("ERROR LINKING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    MVPID = glGetUniformLocation(tableShader->shaderprogram, "MVPMatrix");
    if(MVPID != -1)
    	printf("MVPID: %i\n", MVPID);
    else
    	printf("MVPID: -1, erro na variável uniforme.\n");
}

void display(){
	//Limpar display com um azul escuro
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	tableShader->startUsing();

	glm::mat4 ModelMatrix = glm::mat4(1.0f);            //inicializar matriz modelo
	glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(2, 2, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));


	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
/*void glDrawElements( 	GLenum mode,	->Desenhar triangulos
					  	GLsizei count,	->Desenhar N vertices
  						GLenum type,	->Indexação definida por unsigned ints
					  	const void * indices	-> estrutura de dados com indices, ou quando usamos EBO, o seu offset
					  	);*/
	glDrawElements(GL_TRIANGLES, 2 * 3 * 8, GL_UNSIGNED_INT, 0);
	glFlush();	
}

void idle(){
	glutPostRedisplay();
}
void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            tableShader->cleanup();
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

    tableShader = new DEECShader;
    glutSetWindowTitle("Chess Table");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}
