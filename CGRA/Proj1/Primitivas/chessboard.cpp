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
	printf("TESTE PRIMITIVAS: Chessboard\n");
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
	static quadrado chessboard;
	if(!initFlag){
		chessboard.setImageTexture("")
	}
}


void keyPressed( unsigned char key, int x, int y ) {
    switch(key){
        case 'q':
            printf("\nGoodbye.\n");
            shaderProg->cleanup();
            exit(EXIT_SUCCESS);
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
	glutSetWindowTitle("CGRA: Chessboard");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyPressed);
	init();
	glutMainLoop();
}