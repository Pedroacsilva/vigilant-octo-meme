#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "deecshader.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


GLuint VAOId_cube;
GLuint VBO_cube_position;
GLuint VBO_cube_colors;

DEECShader * myprog;

const GLuint NumVertices = 6 * 2 * 3;       //Vamos desenhar um cubo como um conjunto de triangulos. Um cubo tem 6 faces, constituidas por 2 triangulos, cada um com 3 vértices
//COORDENADAS DOS VERTICES DO CUBO
GLfloat vertices[NumVertices][3] = {
    //face 1 triangle 1
    {-1.0f,-1.0f,-1.0f}, 
    {-1.0f,-1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    //face 1 triangle 2 
    {1.0f, 1.0f,-1.0f}, 
    {-1.0f,-1.0f,-1.0f},
    {-1.0f, 1.0f,-1.0f}, 
    //face 2 triangle 1
    {1.0f,-1.0f, 1.0f},
    {-1.0f,-1.0f,-1.0f},
    {1.0f,-1.0f,-1.0f},
    //face 2 triangle 2
    {1.0f, 1.0f,-1.0f},
    {1.0f,-1.0f,-1.0f},
    {-1.0f,-1.0f,-1.0f},
    //face 3 triangle 1
    {-1.0f,-1.0f,-1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f,-1.0f},
    //face 3 triangle 2
    {1.0f,-1.0f, 1.0f},
    {-1.0f,-1.0f, 1.0f},
    {-1.0f,-1.0f,-1.0f},
    //face 4 triangle 1
    {-1.0f, 1.0f, 1.0f},
    {-1.0f,-1.0f, 1.0f},
    {1.0f,-1.0f, 1.0f},
    //face 4 triangle 2
    {1.0f, 1.0f, 1.0f},
    {1.0f,-1.0f,-1.0f},
    {1.0f, 1.0f,-1.0f},
    //face 5 triangle 1
    {1.0f,-1.0f,-1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f,-1.0f, 1.0f},
    //face 5 triangle 2
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f,-1.0f},
    {-1.0f, 1.0f,-1.0f},
    //face 6 triangle 1
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f,-1.0f},
    {-1.0f, 1.0f, 1.0f},
    //face 6 triangle 2
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    {1.0f,-1.0f, 1.0f}
};

//COR DOS VERTICES DO CUBO
GLfloat vertices_cor[NumVertices][3] ={
    //face 1 triangle 1 (BRANCO)
    {1.0f, 1.0f, 1.0f}, 
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    //face 1 triangle 2 (VERMELHO)
    {1.0f, 0.0f, 0.0f}, 
    {1.0f, 0.0f, 0.0f}, 
    {1.0f, 0.0f, 0.0f},  
    //face 2 triangle 1 (VERDE)
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    //face 2 triangle 2 (AZUL)
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    //face 3 triangle 1 (R + G)
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    //face 3 triangle 2 (R + B)
    {1.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    //face 4 triangle 1 (G + B)
    {0.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    //face 4 triangle 2 (CINZENTO)
    {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    //face 5 triangle 1 (PRETO)
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    //face 5 triangle 2 (MAROON)
    {0.5f, 0.0f, 0.0f},
    {0.5f, 0.0f, 0.0f},
    {0.5f, 0.0f, 0.0f},
    //face 6 triangle 1 (NAVY)
    {0.0f, 0.0f, 0.5f},
    {0.0f, 0.0f, 0.5f},
    {0.0f, 0.0f, 0.5f},
    //face 6 triangle 2 (TEAL)
    {0.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f}

};



void init(){
    glGenVertexArrays(1, &VAOId_cube);
    glBindVertexArray(VAOId_cube);         //Trabalhar no VAO Triangles (indice 0)
    glGenBuffers(1, &VBO_cube_position);          //Gerar simbolos para 1 VBOs no device, armazenados em Buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube_position);        //Trabalhar no buffer de indice 0
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);   //Criar buffer e transferir dados dos vertices, indicando que nao iram mudar (STATIC DRAW)
    glGenBuffers(1, &VBO_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cor), vertices_cor, GL_STATIC_DRAW);
    myprog->loadShaders("cubo.vert","cubo.frag");     //Indicar os shaders com que trabalhar
    myprog->linkShaderProgram();

}

void display(){
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);     //cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube_position);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube_colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glFlush();
}


void keyPressed( unsigned char key, int x, int y ) {
}

void idle(void){
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
    myprog = new DEECShader;
    glutKeyboardFunc( keyPressed );
    glutIdleFunc( idle );       //Definir callback function para no events (idle)
    glutSetWindowTitle("LAB 4 Cube.");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
