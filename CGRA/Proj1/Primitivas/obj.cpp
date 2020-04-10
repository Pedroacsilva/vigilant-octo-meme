#include"obj.h"


genericModel::genericModel(){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboPositions);
    glGenBuffers(1, &vboColors);
    glGenBuffers(1, &vboTex);
}


//Implementar Constructor
Square::Square(){
    printf("SQUARE CONSTRUCTOR.\n");
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboPositions);
    glGenBuffers(1, &vboColors);
    glGenBuffers(1, &vboTex);
    //Quadrado de lado = 1 e com vértices no plano Z = 0
    GLfloat vPositions[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        //----------------------
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };

    //Defeito: Quadrado branco
    GLfloat vColors[] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        //-------------------
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    GLfloat texCoords[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        //----------------------
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
    NumVertices = 6;
    //Encher buffers + definir layout dos dados 
    glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
    printf("Square Ctor: Buffer Positions\n");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vColors), vColors, GL_STATIC_DRAW);
    printf("Square Ctor: Buffer Colors\n");
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vboTex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    printf("Square Ctor: Buffer Texture Coords\n");
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(2);

}

//Implementar Square::drawShape()
void Square::drawShape(DEECShader * shaderProg, glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix){
    shaderProg->startUsing();
    glBindVertexArray(vao);
    glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
    GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
    shaderProg->stopUsing();
}