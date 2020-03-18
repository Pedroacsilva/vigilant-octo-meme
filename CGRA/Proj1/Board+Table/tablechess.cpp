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

unsigned int VAOBoard, VAOTable, EBOBoardIndex, EBOTableIndex, VBOBoardVertices, VBOBoardCores, VBOBoardUV, VBOTableVertices, VBOTableIndex, VBOTableColors, ChessTextureID, MVPID;
DEECShader * shader;


//BOARD
const GLfloat BoardVertices[] = {
	0.9f, 0.0f, 0.9f,		//Top Right
	0.9f, 0.0f, -0.9f, 		//Bottom right
	-0.9f, 0.0f, -0.9f,		//Bottom Left
	-0.9f, 0.0f, 0.9f		//Top Left
};

const GLfloat BoardCores[] ={
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0
};

const GLfloat BoardUV[] = {
	4.0f, 4.0f,			//Top Right
	4.0f, 0.0f,			//Bottom Right
	0.0f, 0.0f,			//Bottom Left
	0.0f, 4.0f,			//Top Left
};

const unsigned int BoardIndex[] = {
	1, 0, 3,			//FIRST TRIANGLE
	2, 1, 3				//SECOND TRIANGLE
};



const unsigned char chessboardTexture[] = {			//Textura 2x2. Um tabuleiro de xadrez tem dimensoes 8x8, pelo que vamos propagar esta textura 4 vezes em U e em V
	255, 255, 255, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	255, 255, 255, 0
};

//TABLE


const GLfloat TableVertices[] ={
	//PERNA: BASE
	-0.2f, -0.5f, -0.2f,
	0.2f, -0.5f, -0.2f,
	0.2f, -0.5f, 0.2f,
	-0.2f, -0.5f, 0.2f,
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


const GLfloat TableCores[]={
	//8 vértices da perna: Saddle Brown, ver https://www.rapidtables.com/web/color/RGB_Color.html
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
	0.55f, 0.27f, 0.07f,
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


const unsigned int TableIndex[] ={
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
//TAMPO:
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
	printf("WORK STUDY: Board + Table\n");
	//Ligar Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	//Começar com board
	glGenVertexArrays(1, &VAOBoard);
	glGenBuffers(1, &VBOBoardVertices);
	glGenBuffers(1, &VBOBoardCores);
	glGenBuffers(1, &VBOBoardUV);
	glGenBuffers(1, &EBOBoardIndex);
	glBindVertexArray(VAOBoard);

	//Transferir coordenadas dos vértices para respectivo VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOBoardVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BoardVertices), BoardVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	//Transferir cores dos vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBOBoardCores);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BoardCores), BoardCores, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(1);
	//Transferir coordenadas texturas
	glBindBuffer(GL_ARRAY_BUFFER, VBOBoardUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BoardUV), BoardUV, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(2);
	//Transferir indexação das coordenadas vértices usando o respectivo EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBoardIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BoardIndex), BoardIndex, GL_STATIC_DRAW);

	//Carregar textura
	glGenTextures(1, &ChessTextureID);
	glBindTexture(GL_TEXTURE_2D, ChessTextureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessboardTexture);		//Enviar textura para estrutura de dados
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Table
	glGenVertexArrays(1, &VAOTable);
	glGenBuffers(1, &VBOTableVertices);
	glGenBuffers(1, &VBOTableColors);
	glGenBuffers(1, &EBOTableIndex);
	glBindVertexArray(VAOTable);
	glBindBuffer(GL_ARRAY_BUFFER, VBOTableVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TableVertices), TableVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOTableColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TableCores), TableCores, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTableIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TableIndex), TableIndex, GL_STATIC_DRAW);

	if(shader->loadShaders("shader.vert","shader.frag") == GL_FALSE){
        printf("ERROR LOADING SHADERS.\n");
        exit(EXIT_FAILURE);
    }
    if(shader->linkShaderProgram() == GL_FALSE){
        printf("ERROR LINKING SHADERS.\n");
        exit(EXIT_FAILURE);
    }


}
void display(){
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAOBoard);
	shader->startUsing();
	glm::mat4 ModelMatrix = glm::mat4(1.0f);            //inicializar matriz modelo
	glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);
	//Mudar para VAO da mesa
	glBindVertexArray(VAOTable);
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
            shader->cleanup();
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

    shader = new DEECShader;
    glutSetWindowTitle("Table + Chess");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}
