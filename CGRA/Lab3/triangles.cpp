#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

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


#include <stdio.h>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
const GLuint NumVertices = 6;
GLfloat  vertices[NumVertices][2] = {{ -0.90, -0.90 },{  0.85, -0.90 }, { -0.90,  0.85 },// triangle 1
				     {  0.90, -0.85 }, {  0.90,  0.90 },{ -0.85,  0.90 } // triangle 2
};

DEECShader * myprog;

void init() {
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
    myprog->loadShaders("triangles.vert","triangles.frag");
    myprog->linkShaderProgram();
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,  GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}

void display() {
    myprog->startUsing();
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
    myprog->stopUsing();
}

void keyPressed( unsigned char key, int x, int y ) {
  switch( key ) {
  case 27:
  case 'q' :
  case 'Q' :
    exit( EXIT_SUCCESS );
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
    glutKeyboardFunc( keyPressed );

    myprog = new DEECShader;
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
