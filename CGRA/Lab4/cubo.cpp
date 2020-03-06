#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "deecshader.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

GLuint VAOId_cube, VBO_cube_position, MVPID, CubePosID;
unsigned int timeVariable, newTime, oldTime, deltaTime;

DEECShader * myprog;

const GLuint NumVertices = 6 * 2 * 3;       //Vamos desenhar um cubo como um conjunto de triangulos. Um cubo tem 6 faces, constituidas por 2 triangulos, cada um com 3 vértices
//COORDENADAS DOS VERTICES DO CUBO
static const GLfloat vertices[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
     }; 


GLfloat cube_centers[] = {
    0.0, 0.0, 0.0,          //Primeiro cubo em (0, 0, 0)
    0.0, 2.0, 0.0,          //Segundo cubo em (0, 2, 0)
    2.0, 0.0, 0.0           //Terceiro cubo em (2, 0, 0) 
};

GLfloat cores[NumVertices];





void init(){
    glGenVertexArrays(1, &VAOId_cube);      //Gerar VAO para cubo
    glBindVertexArray(VAOId_cube);         //Trabalhar no VAO do cubo
    glGenBuffers(1, &VBO_cube_position);          //Gerar buffer de dados para coordenadas dos vertices do cubo
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube_position);          //Trabalhar no buffer de coordenadas dos vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);       //Escrever no buffer*/
    myprog->loadShaders("cubo.vert","cubo.frag");     //Indicar os shaders com que trabalhar
    myprog->linkShaderProgram();
    MVPID = glGetUniformLocation(myprog->shaderprogram, "MVPMatrix");
    //CubePosID = glGetUniformLocation(myprog->shaderprogram, "CubePos");
    glVertexAttribPointer(
                            0,          //localizacao do atributo. Deve ser concordante com layout do shader
                            3,          //tamanho dos dados
                            GL_FLOAT,   //tipo dos dados
                            GL_FALSE,   //normalizado?
                            0,          //stride
                            BUFFER_OFFSET(0)    //offset
                            );
    glEnableVertexAttribArray(0);
    //Inicializar cores
    for(int i = 0; i < NumVertices * 3; i = i + 3){
        cores[i] = rand() % 255 / 255.0;            //R
        cores[i + 1] = rand() % 255 / 255.0;        //G
        cores[i + 2] = rand() % 255 / 255.0;        //B
    }
    //Transferir cores para device
    glVertexAttribPointer(
                            1,
                            2,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            BUFFER_OFFSET(0)
                            );
    glEnableVertexAttribArray(1);

    for(int i = 0; i < 36; i = i + 3)
        printf("Vertice %i: (%f, %f, %f)\n", i , vertices[i], vertices[i+1], vertices[i+2]);
    for(int i = 0; i < NumVertices ; i = i + 3)
        printf("Cor Vertice %i: (%f, %f, %f)\n", i, cores[i], cores[i + 1], cores[i + 2]);
    printf("MVPMatrix: %i\n", MVPID);
    //printf("CubePosID: %i\n", CubePosID);
    if(glIsBuffer(VBO_cube_position) == GL_TRUE)
        printf("VBO_cube_position é um buffer.\n");
    else
        printf("VBO_cube_position não é um buffer.\n");


}

void display(){
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);     //cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);            //Ligar depth test para fragmentos correctos serem desenhados (e nao simplesmente o mais recente)
    glDepthFunc(GL_LESS);               //Criterio de teste: passa o fragmento mais perto da camara
    myprog->startUsing();

    //glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));         //Transpor vertices : o cubo esteja centrado em (10, 10, 10)
    //glm::vec4 CubePosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);             //Centro do cubo
    glm::vec3 CubePosition = glm::vec3(0.0f, 0.0f, 0.0f);                       //centro do cubo
    glm::mat4 ModelMatrix = glm::mat4(1.0f);            //inicializar matriz modelo
    /*ModelMatrix1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    ModelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 2.0, 0.0));
    ModelMatrix3 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0, 0.0, 0.0));*/
    ModelMatrix = glm::translate(ModelMatrix, CubePosition);            //translação para centro do cubo
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));      //Aplicar translação
    ModelMatrix = glm::rotate(ModelMatrix, (float) timeVariable / 1000, glm::vec3(0.0f, 1.0f, 0.0f));      //Aplicar rotação sobre o eixo Y
    /*ModelMatrix1 = glm::rotação(ModelMatrix1, (float) timeVariable / 1000, glm::vec3(1.0f, 0.0f, 0.0f));
    ModelMatrix2 = glm::rotação(ModelMatrix2, (float) timeVariable / 1000, glm::vec3(0.0f, 1.0f, 0.0f));
    ModelMatrix3 = glm::rotação(ModelMatrix3, (float) timeVariable / 1000, glm::vec3(0.0f, 0.0f, 1.0f));*/
    glm::mat4 ViewMatrix       = glm::lookAt(
                                glm::vec3(4, 3, -3), // Camera na posição (4, 3, -3) (coordenadas mundo)
                                //glm::vec3(0, 0, 0), // observa o cubo (na origem)
                                glm::vec3(10, 10, 10),  //observa o cubo (em 10,10,10)
                                glm::vec3(0, 1, 0)  // camara de pé
                                );
    glm::mat4 ProjectionMatrix = glm::perspective(
                                glm::radians(45.0f),        //Field of View
                                4.0f / 3.0f,                //Aspect Ratio
                                0.1f,                       //nearplane
                                100.0f                       //farplane
                                );
    glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(
                        MVPID,              //localizacao dos dados nos shaders
                        1,                  //queremos modificar 1 matriz (e nao um array de matrizes)
                        GL_FALSE,           //nao enviar a matriz transposta
                        glm::value_ptr(MVPMatrix)
                        );
    glBindVertexArray(VAOId_cube);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDisableVertexAttribArray(0);
    glFlush();
    myprog->stopUsing();
}


void keyPressed( unsigned char key, int x, int y ) {
}

void idle(void){
    deltaTime = newTime - oldTime;
    timeVariable += deltaTime;
    oldTime = newTime;
    newTime = glutGet(GLUT_ELAPSED_TIME);
    printf("\rTime: %i.", timeVariable);
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
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
