using namespace std;
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "deecshader.h"
#include "obj.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


bool initFlag = false;
unsigned int timeVariable, newTime, oldTime, deltaTime;
DEECShader * shaderProg;

void init(){
	printf("TESTE PRIMITIVAS\n");
	if(shaderProg->loadShaders("shader.vert","shader.frag") == GL_FALSE){
		printf("ERROR LOADING SHADERS.\n");
		exit(EXIT_FAILURE);
	}
	if(shaderProg->linkShaderProgram() == GL_FALSE){
		printf("ERROR LINKING SHADERS.\n");
		exit(EXIT_FAILURE);
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void display(){
	static Cylinder cubo;
	if(!initFlag){
		cubo.setImageTexture("chessboard.ppm");
		cubo.setVertexColor(1.0f, 0.0f, 1.0f);
		initFlag = !initFlag;
	}
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));		//Translação para (0, 0, 10)
	glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(4, 0, 0), glm::vec3(0, 0, -10), glm::vec3(0, 1, 0));
	glm::mat4 PerspectiveMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 7.0f, 14.0f);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)timeVariable/10), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MVPMatrix = PerspectiveMatrix * ViewMatrix * ModelMatrix;
	cubo.setMVPMatrix(MVPMatrix);
	cubo.drawShape(shaderProg);
	glFlush();
}

void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            shaderProg->cleanup();
            exit(EXIT_SUCCESS);
            break;
        case 'l':
        	printf("Depth Test: GL_LESS");
        	glDepthFunc(GL_LESS);
        	break;
        case 'g':
        	printf("Depth Test: GL_GREATER");
        	glDepthFunc(GL_GREATER);
        	break;
        case 'd':
        	printf("Depth Test disabled.\n");
        	glDisable(GL_DEPTH_TEST);
        	break;
        case 'e':
        	printf("Depth Test enabled.\n");
        	glEnable(GL_DEPTH_TEST);
        	break;
        case 'n':
            printf("Depth Test: GL_NEVER.\n");
        	glDepthFunc(GL_NEVER);
        	break;
        case 'a':
            printf("Depth Test: GL_ALWAYS\n");
        	glDepthFunc(GL_ALWAYS);
        	break;
    }
}

void idle(void){
	deltaTime = newTime - oldTime;
    timeVariable += deltaTime;
    oldTime = newTime;
    newTime = glutGet(GLUT_ELAPSED_TIME);
    //printf("\rTime: %i ms.", timeVariable);
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_RGBA|GLUT_3_2_CORE_PROFILE);
#else // LINUX
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
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

	shaderProg = new DEECShader;
	glutSetWindowTitle("OOP OpenGL: Square");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyPressed);
	init();
	glutMainLoop();
}