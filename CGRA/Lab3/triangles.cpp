/*CGRA 2019_2020
Lab 3: Desenho de triangulos, múltiplos shaders
Autor: Pedro Silva (2011149228)*/

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
#include <math.h>
#include <stdlib.h>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

int timevariable, newtime, oldtime, deltatime = 0;
int timeLoc[4]; 
int colorLoc[4];
int shader_switch = 0;
float color_shader[3];
GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
const GLuint NumVertices = 6;
GLfloat  vertices[NumVertices][2] = {{ -0.90, -0.90 },{  0.85, -0.90 }, { -0.90,  0.85 },// triangle 1
				     {  0.90, -0.85 }, {  0.90,  0.90 },{ -0.85,  0.90 } // triangle 2
};

DEECShader * myprog, * prog3_2, * prog3_3, * prog3_4;
/*
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
*/
void init() {
    glGenVertexArrays(NumVAOs, VAOs);           //Gerar simbolos para VAO. NumVAOs (1) armazenados no array VAOs
    glBindVertexArray(VAOs[Triangles]);         //Trabalhar no VAO Triangles (indice 0)
    glGenBuffers(NumBuffers, Buffers);          //Gerar simbolos para 1 VBOs no device, armazenados em Buffers
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);        //Trabalhar no buffer de indice 0
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);   //Criar buffer e transferir dados dos vertices, indicando que nao iram mudar (STATIC DRAW)

    myprog->loadShaders("triangles.vert","triangles_color.frag");     //Indicar os shaders com que trabalhar
    myprog->linkShaderProgram();

    prog3_2->loadShaders("triangles_y.vert", "triangles_color.frag");
    prog3_2->linkShaderProgram();

    prog3_3->loadShaders("triangles_scale.vert", "triangles_color.frag");
    prog3_3->linkShaderProgram();

    prog3_4->loadShaders("triangles_rotate.vert", "triangles_color.frag");
    prog3_4->linkShaderProgram();

    timeLoc[0] = glGetUniformLocation(myprog->shaderprogram, "timepar");
    colorLoc[0] = glGetUniformLocation(myprog->shaderprogram, "color_change");

    timeLoc[1] = glGetUniformLocation(prog3_2->shaderprogram, "timepar");
    colorLoc[1] = glGetUniformLocation(prog3_2->shaderprogram, "color_change");

    timeLoc[2] = glGetUniformLocation(prog3_3->shaderprogram, "timepar");
    colorLoc[2] = glGetUniformLocation(prog3_3->shaderprogram, "color_change");

    timeLoc[3] = glGetUniformLocation(prog3_4->shaderprogram, "timepar");
    colorLoc[3] = glGetUniformLocation(prog3_4->shaderprogram, "color_change");

    glVertexAttribPointer(vPosition, 2, GL_FLOAT,  GL_FALSE, 0, BUFFER_OFFSET(0));      //especificar formato de dados do VBO (stride, offset, etc...)
    glEnableVertexAttribArray(vPosition);               //Ligar o VAO vPosition
}


/*
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
*/
void display() {
    /*myprog->startUsing();
    glClear(GL_COLOR_BUFFER_BIT);           //Limpar color buffer
    glBindVertexArray(VAOs[Triangles]);        //Trabalhar no VAO Triangles
    glUniform1f(timeLoc[0], timevariable);
    glUniform3f(colorLoc[0], color_shader[0], color_shader[1], color_shader[2]);

    glUniform1f(timeLoc[1], timevariable);
    glUniform3f(colorLoc[1], color_shader[0], color_shader[1], color_shader[2]);

    glUniform1f(timeLoc[2], timevariable);
    glUniform3f(colorLoc[2], color_shader[0], color_shader[1], color_shader[2]);

    glUniform1f(timeLoc[3], timevariable);
    glUniform3f(colorLoc[3], color_shader[0], color_shader[1], color_shader[2]);


    glDrawArrays(GL_TRIANGLES, 0, NumVertices);     //Desenhar Triangulos, o array com indice 0 e desenhar NumVertices
    glFlush();
    myprog->stopUsing();*/
    /*myprog->startUsing();
    glClear(GL_COLOR_BUFFER_BIT);           //Limpar color buffer
    glBindVertexArray(VAOs[Triangles]);        //Trabalhar no VAO Triangles
    glUniform1f(timeLoc[0], timevariable);
    glUniform3f(colorLoc[0], color_shader[0], color_shader[1], color_shader[2]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);     //Desenhar Triangulos, o array com indice 0 e desenhar NumVertices
    glFlush();
    myprog->stopUsing();*/
    switch(shader_switch){
        case 0:{
            myprog->startUsing();
            glClear(GL_COLOR_BUFFER_BIT);           //Limpar color buffer
            glBindVertexArray(VAOs[Triangles]);        //Trabalhar no VAO Triangles
            glUniform1f(timeLoc[0], timevariable);
            glUniform3f(colorLoc[0], color_shader[0], color_shader[1], color_shader[2]);
            glDrawArrays(GL_TRIANGLES, 0, NumVertices);     //Desenhar Triangulos, o array com indice 0 e desenhar NumVertices
            glFlush();
            myprog->stopUsing();
            break;
        }
        case 1:{
            prog3_2->startUsing();
            glClear(GL_COLOR_BUFFER_BIT);           //Limpar color buffer
            glBindVertexArray(VAOs[Triangles]);        //Trabalhar no VAO Triangles
            glUniform1f(timeLoc[1], timevariable);
            glUniform3f(colorLoc[1], color_shader[0], color_shader[1], color_shader[2]);
            glDrawArrays(GL_TRIANGLES, 0, NumVertices);     //Desenhar Triangulos, o array com indice 0 e desenhar NumVertices
            glFlush();
            prog3_2->stopUsing();
            break;
        }
        case 2:{
            prog3_3->startUsing();
            glClear(GL_COLOR_BUFFER_BIT);           //Limpar color buffer
            glBindVertexArray(VAOs[Triangles]);        //Trabalhar no VAO Triangles
            glUniform1f(timeLoc[2], timevariable);
            glUniform3f(colorLoc[2], color_shader[0], color_shader[1], color_shader[2]);
            glDrawArrays(GL_TRIANGLES, 0, NumVertices);     //Desenhar Triangulos, o array com indice 0 e desenhar NumVertices
            glFlush();
            prog3_3->stopUsing();
            break;
        }
        case 3:{
            prog3_4->startUsing();
            glClear(GL_COLOR_BUFFER_BIT);           //Limpar color buffer
            glBindVertexArray(VAOs[Triangles]);        //Trabalhar no VAO Triangles
            glUniform1f(timeLoc[3], timevariable);
            glUniform3f(colorLoc[3], color_shader[0], color_shader[1], color_shader[2]);
            glDrawArrays(GL_TRIANGLES, 0, NumVertices);     //Desenhar Triangulos, o array com indice 0 e desenhar NumVertices
            glFlush();
            prog3_4->stopUsing();
            break;
        }
    }
}

void keyPressed( unsigned char key, int x, int y ) {
  switch( key ) {
  case 27:
  case 'q' :
  case 'Q' :{
    myprog->cleanup();
    prog3_2->cleanup();
    prog3_3->cleanup();
    prog3_4->cleanup();
    exit( EXIT_SUCCESS );
    break;
    }
  case 's':{
    if(shader_switch != 3)
        shader_switch++;
    else
        shader_switch = 0;
    printf("\nEVENT: shader changed: %i.\n", shader_switch);
    break;
  }
  /*case 'y' :            //Mudar shader para variar y sinusoidalmente
        break;
  case 's' :            //Usar shader para scaling
        break;
  case 'r' :            //Usar shader para rotacao
        break;*/
  /*case 'c' :            //Trocar fragment shader
    color_shader = 1;
    printf("\rtimevariable: %i. Toggled fragment shader: %i.", timevariable, color_shader);
    break;
  case 'g'  :
    color_shader = 0;
    printf("\rtimevariable: %i. Toggled fragment shader: %i.", timevariable, color_shader);
    break;*/


  }
}

void idle(void){
    deltatime = newtime - oldtime;
    oldtime = newtime;
    timevariable += deltatime;
    newtime = glutGet(GLUT_ELAPSED_TIME);
    /*color_shader[0] = sin(newtime/100 * 360);
    color_shader[1] = sin(newtime/200 * 360);
    color_shader[2] = sin(newtime/300 * 360);*/
    /*color_shader[0] = rand() % 255 / 255.0;
    color_shader[1] = rand() % 255 / 255.0;
    color_shader[2] = rand() % 255 / 255.0;*/
    color_shader[0] = (newtime / 100) % 255 / 255.0;
    color_shader[1] = (newtime / 50) % 255 / 255.0;
    color_shader[2] = (newtime / 25) % 255 / 255.0;
  printf("\rtimevariable: %i. R = %f. G = %f. B = %f.", timevariable, color_shader[0], color_shader[1], color_shader[2]); 
  glutPostRedisplay();        //Marca que a janela actual deve ser redesenhada.

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
    glutIdleFunc( idle );       //Definir callback function para no events (idle)
    glutSetWindowTitle("LAB 3 Triangles.");
    myprog = new DEECShader;
    prog3_2 = new DEECShader;
    prog3_3 = new DEECShader;
    prog3_4 = new DEECShader;
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
