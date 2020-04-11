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


Square quadrado;
DEECShader * shaderProg;

void init(){
	printf("TESTE PRIMITIVAS: QUADRADO.\n");
	if(shaderProg->loadShaders("shader.vert","shader.frag") == GL_FALSE){
		printf("ERROR LOADING SHADERS.\n");
		exit(EXIT_FAILURE);
	}
	if(shaderProg->linkShaderProgram() == GL_FALSE){
		printf("ERROR LINKING SHADERS.\n");
		exit(EXIT_FAILURE);
	}
}

void display(){
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	quadrado.drawShape(shaderProg);
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

	shaderProg = new DEECShader;
	glutSetWindowTitle("OOP OpenGL: Square");
	init();
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	//glutKeyboardFunc(keyPressed);
	glutMainLoop();
}