#include"obj.h"

/********************************************************
*                           SQUARE                      *
********************************************************/
//Implementar Constructor
Square::Square(){
    printf("Square constructor called.\n");
    NumVertices = 4;
    //Alocar memória
    vCoords = new GLfloat[NumVertices * 3];		//Coordenadas
    vColors = new GLfloat[NumVertices * 3];		//Cores
    vTexCoords = new GLfloat[NumVertices * 2];	//Texturas
    vEBO = new GLuint[2 * 3];             //EBO: Por quantos triangulos o nosso poligono e constituido * 3
    //Inicializar coordenadas dos vértices		(XYZ)
    vCoords[0] = -1.0f; vCoords[1] = 1.0f ; vCoords[2] = 1.0f;
    vCoords[3] = -1.0f; vCoords[4] = -1.0f; vCoords[5] = 1.0f;
    vCoords[6] = 1.0f ; vCoords[7] = -1.0f; vCoords[8] = 1.0 ;
    vCoords[9] = 1.0f ; vCoords[10] = 1.0f; vCoords[11] = 1.0;
    //Inicializar cores (RGB). Por defeito, o quadrado é branco
    for(int i = 0; i < NumVertices * 3; i++)
    	vColors[i] = 1.0f;
    //Inicalizar coordenadas textura (UV)
    vTexCoords[0] = 0.0f ; vTexCoords[1] = 1.0f ;
	vTexCoords[2] = 0.0f ; vTexCoords[3] = 0.0f ;
	vTexCoords[4] = 1.0f ; vTexCoords[5] = 0.0f ;
	vTexCoords[6] = 1.0f ; vTexCoords[7] = 1.0f	;
	//Inicializar EBO
	vEBO[0] = 0; vEBO[1] = 1; vEBO[2] = 3;
	vEBO[3] = 1; vEBO[4] = 2; vEBO[5] = 3;
}
//Destructor
Square::~Square(){
    delete vCoords;
    delete vColors;
    delete vTexCoords;
    delete vEBO;
    printf("Square destructor called.\n");
}

//Implementar Square::drawShape()
void Square::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
	//Gerar e bind VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Gerar VBOs
	unsigned int vboCoords, vboColors, vboTexCoords, vboEBO;
	glGenBuffers(1, &vboCoords);
	glGenBuffers(1, &vboColors);
	glGenBuffers(1, &vboTexCoords);
	glGenBuffers(1, &vboEBO);
    //Gerar + Bind Texture ID 
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);  
	shaderProg->startUsing();
    //Localizar variável uniforme
    GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
    //printf("[Square::drawShape()]MVPID: %i\n", MVPID);
    //Transferir matriz ModelVieWProjection
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));     
	//Encher buffers + definir layout dos dados 
    glBindBuffer(GL_ARRAY_BUFFER, vboCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, vCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, vColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 4, vTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(2);
    

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, vEBO, GL_STATIC_DRAW);
   	//Desenhar triangulos na ordem do EBO
   	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    shaderProg->stopUsing();
    glFlush();
}


/********************************************************
*                           CUBE                        *
********************************************************/

//Implementar Constructor & Destructor
Cube::Cube(){
    printf("Cube constructor called.\n");
    NumVertices = 8;
    //Alocar memória
    vCoords = new GLfloat[NumVertices * 3];     //Coordenadas
    vColors = new GLfloat[NumVertices * 3];     //Cores
    vTexCoords = new GLfloat[NumVertices * 2];  //Texturas
    vEBO = new GLuint[2 * 3 * 6];
    //Inicializar coordenadas dos vértices      (XYZ)
    //Face Z = 1.0
    vCoords[0] = -1.0f; vCoords[1] = 1.0f ; vCoords[2] = 1.0f;
    vCoords[3] = -1.0f; vCoords[4] = -1.0f; vCoords[5] = 1.0f;
    vCoords[6] = 1.0f ; vCoords[7] = -1.0f; vCoords[8] = 1.0 ;
    vCoords[9] = 1.0f ; vCoords[10] = 1.0f; vCoords[11] = 1.0;
    //Face Z = -1.0
    vCoords[12] = -1.0f; vCoords[13] = 1.0f ; vCoords[14] = -1.0f;
    vCoords[15] = -1.0f; vCoords[16] = -1.0f; vCoords[17] = -1.0f;
    vCoords[18] = 1.0f ; vCoords[19] = -1.0f; vCoords[20] = -1.0 ;
    vCoords[21] = 1.0f ; vCoords[22] = 1.0f; vCoords[23] = -1.0;
    //Inicializar cores (RGB). Por defeito, o quadrado é branco
    for(int i = 0; i < NumVertices * 3; i++)
        vColors[i] = 1.0f;
    //Inicalizar coordenadas textura (UV)
    vTexCoords[0] = 0.0f ; vTexCoords[1] = 1.0f ;
    vTexCoords[2] = 0.0f ; vTexCoords[3] = 0.0f ;
    vTexCoords[4] = 1.0f ; vTexCoords[5] = 0.0f ;
    vTexCoords[6] = 1.0f ; vTexCoords[7] = 1.0f ;
    vTexCoords[8] = 0.0f ; vTexCoords[9] = 1.0f ;
    vTexCoords[10] = 0.0f ; vTexCoords[11] = 0.0f ;
    vTexCoords[12] = 1.0f ; vTexCoords[13] = 0.0f ;
    vTexCoords[14] = 1.0f ; vTexCoords[15] = 1.0f ;
    //Inicializar EBO
    //Face Z = 1.0
    vEBO[0] = 0; vEBO[1] = 1; vEBO[2] = 3;
    vEBO[3] = 1; vEBO[4] = 2; vEBO[5] = 3;
    //Face Z = -1.0
    vEBO[6] = 4; vEBO[7] = 5; vEBO[8] = 7;
    vEBO[9] = 5; vEBO[10] = 6; vEBO[11] = 7;
    //Face X = -1.0
    vEBO[12] = 4; vEBO[13] = 5; vEBO[14] = 0;
    vEBO[15] = 5; vEBO[16] = 1; vEBO[17] = 0;
    //Face X = 1.0
    vEBO[18] = 7; vEBO[19] = 6; vEBO[20] = 3;
    vEBO[21] = 6; vEBO[22] = 2; vEBO[23] = 3;
    //Face Y = 1.0
    vEBO[24] = 0; vEBO[25] = 4; vEBO[26] = 3;
    vEBO[27] = 4; vEBO[28] = 7; vEBO[29] = 3;
    //Face Y = -1.0
    vEBO[30] = 0; vEBO[31] = 4; vEBO[32] = 3;
    vEBO[33] = 4; vEBO[34] = 7; vEBO[35] = 3;
    //Gerar e bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //Gerar VBOs
    unsigned int vboCoords, vboColors, vboTexCoords, vboEBO;
    glGenBuffers(1, &vboCoords);
    glGenBuffers(1, &vboColors);
    glGenBuffers(1, &vboTexCoords);
    glGenBuffers(1, &vboEBO);
    //Gerar + Bind Texture ID 
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
        //Encher buffers + definir layout dos dados 
    glBindBuffer(GL_ARRAY_BUFFER, vboCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NumVertices, vCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NumVertices, vColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * NumVertices, vTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(2);
    

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * 2 * 6, vEBO, GL_STATIC_DRAW);

}
//Destructor
Cube::~Cube(){
    delete vCoords;
    delete vColors;
    delete vTexCoords;
    delete vEBO;
    printf("Cube destructor called.\n");
}


void Cube::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    shaderProg->startUsing();
    //Localizar variável uniforme
    GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
    //printf("[Cube::drawShape()]MVPID: %i\n", MVPID);
    //Transferir matriz ModelVieWProjection
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));     
    //Desenhar triangulos na ordem do EBO
    glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    shaderProg->stopUsing();
    glFlush();
}

void Cube::setImageTexture(char * textureName){
    //Vou utilizar um cube map para definir a textura do cubo. 
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    for(int i = 0; i < 6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}