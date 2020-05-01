#include"obj.h"
#include<math.h>
#define PI 3.141592
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
    vCoords[0] = -1.0f; vCoords[1] = 1.0f ; vCoords[2] = 0.0f;
    vCoords[3] = -1.0f; vCoords[4] = -1.0f; vCoords[5] = 0.0f;
    vCoords[6] = 1.0f ; vCoords[7] = -1.0f; vCoords[8] = 0.0 ;
    vCoords[9] = 1.0f ; vCoords[10] = 1.0f; vCoords[11] = 0.0;
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
    //Gerar e bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //Gerar VBOs
    glGenBuffers(1, &vboCoords);
    glGenBuffers(1, &vboColors);
    glGenBuffers(1, &vboTexCoords);
    glGenBuffers(1, &vboEBO);
    //Gerar 
    glGenTextures(1, &texID);
//    glBindTexture(GL_TEXTURE_2D, texID);  
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
}
//Destructor
Square::~Square(){
    printf("Square destructor called.\n");
    delete vCoords;
    delete vColors;
    delete vTexCoords;
    delete vEBO;
    //Clean up VAOs e VBOs
    glDeleteBuffers(1, &vboCoords);
    glDeleteBuffers(1, &vboColors);
    glDeleteBuffers(1, &vboTexCoords);
    glDeleteBuffers(1, &vboEBO);
}

//Implementar Square::drawShape()
void Square::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texID);
	shaderProg->startUsing();
    //Localizar variável uniforme
    GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
    //printf("[Square::drawShape()]MVPID: %i\n", MVPID);
    //Transferir matriz ModelVieWProjection
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));     
	//Encher buffers + definir layout dos dados 
   	//Desenhar triangulos na ordem do EBO
   	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    shaderProg->stopUsing();
    //glFlush();
}


/********************************************************
*                           CUBE                        *
********************************************************/

//Implementar Constructor & Destructor
//NOTA: Para texturar um cubo, usar um EBO nao é ideal. Os vértices dum cubo têm coordenadas textura diferentes!!
/*Cube::Cube(){
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
    glBindTexture(GL_TEXTURE_2D, texID);
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
}*/
Cube::Cube(){
    printf("Cube constructor called. Not using EBO.\n");
    NumVertices = 2 * 6 * 3;            //36
    GLfloat vertices[] = {
        -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,

        1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0,
        1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,

        1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0,
        1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0,

        -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0,
        -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0,

        -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0,

        -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0
     };
     GLfloat uvCoord[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    }; 
     vCoords = vertices;
     vTexCoords = uvCoord;
     vColors = new GLfloat[3 * 3 * 2 * 6];
     /*for(int i = 0; i < 3 * 3 * 2 * 6; i++)
        vColors[i] = 1.0f;     //Inicializar o cubo como branco*/
    for(int i = 0; i < NumVertices * 3; i = i + 3){
        vColors[i] = rand() % 255 / 255.0;            //R
        vColors[i + 1] = rand() % 255 / 255.0;        //G
        vColors[i + 2] = rand() % 255 / 255.0;        //B
    }

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
        //Encher buffers + definir layout dos dados 
    glBindBuffer(GL_ARRAY_BUFFER, vboCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NumVertices, vColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoord), vTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(2);
}

//Destructor
Cube::~Cube(){
        printf("Cube destructor called.\n");
        /*delete vCoords;
        delete vColors;
        delete vTexCoords;
        delete vEBO;*/
    //Clean up VAOs e VBOs
        glDeleteBuffers(1, &vboCoords);
        glDeleteBuffers(1, &vboColors);
        glDeleteBuffers(1, &vboTexCoords);
        glDeleteBuffers(1, &vboEBO);
    }


void Cube::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texID);
        shaderProg->startUsing();
    //Localizar variável uniforme
        GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));     
    //Desenhar triangulos na ordem do EBO
//        glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);
        //Se nao estiver a usar o EBO:
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);
        shaderProg->stopUsing();
    }

/*void Cube::setImageTexture(char * textureName){
    //Pelo que vejo um cube map nao e a melhor forma para texturar um cubo (mais para skyboxes?), o blender daria-me as coordenadas UV dum cubo mas lol nao tenho blender porra
    //Vou utilizar um cube map para definir a textura do cubo. 
    /*glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    for(int i = 0; i < 6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/
//}

/********************************************************
*                           CYLINDER                    *
********************************************************/
Cylinder::Cylinder(){
    printf("Cylinder Default Constructor.\n");
    /*Um cilindro é essencialmente um prisma com um numero muito elevado de vertices em cada face extrema.
     Neste constructor, o ciclindro tem faces em y = -1.0 y = 1.0 e raio = 1.0*/
    float raio = 1.0;
    float theta_step = 2 * PI / 10.0;
    NumVertices = 22;
    //Alocar memória
    vCoords = new GLfloat[NumVertices * 3];     //Coordenadas
    vColors = new GLfloat[NumVertices * 3];     //Cores
    vTexCoords = new GLfloat[NumVertices * 2];  //Texturas
    vEBO = new GLuint[NumVertices * 3];             //EBO: Por quantos triangulos o nosso poligono e constituido
    //Definir coordenadas, cores e coordenadas UV dos vertices
    for(int i = 0; i < NumVertices / 2; i++){
        //X, R                                                     Y, G                                        Z, B
        vCoords[i * 3] = cosf(theta_step * i) * raio;               vCoords[i * 3 + 1] = 1.0;                   vCoords[i * 3 + 2] = sinf(theta_step * i) * raio;    //Face y = 1.0
        vCoords[i * 3 + 33] = cosf(theta_step * i) * raio;          vCoords[i * 3 + 34] = -1.0;                 vCoords[i * 3 + 35] = sinf(theta_step * i) * raio;   //face y = -1.0
        vColors[i * 3] = rand() % 255 / 255.0;                     vColors[i * 3 + 1] = rand() % 255 / 255.0;  vColors[i * 3 + 2] = rand() % 255 / 255.0;
        vColors[i * 3 + 33] = rand() % 255 / 255.0;                vColors[i * 3 + 34] = rand() % 255 / 255.0; vColors[i * 3 + 35] = rand() % 255 / 255.0;
        //Como mapear uma textura 2D na face lateral dum cilindro? V = y, U em funcao de i
        vTexCoords[i * 2] = (float) (i) / 10.0;           vTexCoords[i * 2 + 1] = 1.0;
        vTexCoords[i * 2 + 20] = (float) (i) / 10.0;      vTexCoords[i * 2 + 21] = 0.0;
        //printf("i = %i, x(%i) = %f, z(%i) = %f, u(%i) = %f\n", i, i, vCoords[i * 3], i, vCoords[i * 3 + 2], i, vTexCoords[i * 2]);
    }
    //EBO: Ordem dos vértices. Cilindro composto por triangulos. Podemos iterar por um ciclo, mas no case em que i = NumVertices / 2, temos de fazer "à mao"
    for(int i = 0; i < NumVertices / 2; i ++){
        vEBO[i * 6] = i; vEBO[i * 6 + 1] = i + NumVertices / 2; vEBO[i * 6 + 2] = i + NumVertices / 2 + 1;
        vEBO[i * 6 + 3] = i; vEBO[i * 6 + 4] = i + NumVertices / 2 + 1; vEBO[i * 6 + 5] = i + 1;
    }

    //Gerar e bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //Gerar VBOs
    glGenBuffers(1, &vboCoords);
    glGenBuffers(1, &vboColors);
    glGenBuffers(1, &vboTexCoords);
    glGenBuffers(1, &vboEBO);
    //Gerar  textura
    glGenTextures(1, &texID);
    //Preencher buffers
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * NumVertices * 3, vEBO, GL_STATIC_DRAW);
}

//Destructor
Cylinder::~Cylinder(){
    printf("Cylinder destructor called.\n");
    delete vCoords;
    delete vColors;
    delete vTexCoords;
    delete vEBO;
    //Clean up VAOs e VBOs
    glDeleteBuffers(1, &vboCoords);
    glDeleteBuffers(1, &vboColors);
    glDeleteBuffers(1, &vboTexCoords);
    glDeleteBuffers(1, &vboEBO);
}

void Cylinder::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texID);
    shaderProg->startUsing();
    //Localizar variável uniforme
    GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
    glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVPMatrix));     
    //Desenhar triangulos na ordem do EBO
    glDrawElements(GL_TRIANGLES, 3 * 2 * 10, GL_UNSIGNED_INT, 0);
    shaderProg->stopUsing();
}


/********************************************************
*                       SPHERE                          *
********************************************************/
//TODO: Pensar como formar um EBO para a esfera
Sphere::Sphere(){
    printf("Spehere Constructor called.\n");
    /*Vamos construir uma esfera fazendo vários gomos de laranja. Cada gomo vai ser constituido, por defeito,
    por 5 quadrados -> 10 triangulos.*/
    float raio = 1.0;
    float phi, theta = 0.0;
    float phiStep = 2 * PI / 10;
    float thetaStep = PI / 5;
    NumVertices = 60;
    //Alocar memória
    vCoords = new GLfloat[NumVertices * 3];     //Coordenadas
    vColors = new GLfloat[NumVertices * 3];     //Cores
    vTexCoords = new GLfloat[NumVertices * 2];  //Texturas
    vEBO = new GLuint[NumVertices * 3];             //EBO: Por quantos triangulos o nosso poligono e constituido
    //Definir coordenadas, cores e coordenadas UV dos vertices
    /*Vamos usar 10 gomos de laranja, cada um definido por 5 quadrados. Assim teremos de iterar por cada quadrado por cada gomo*/
    /*Equação paramétrica duma esfera:
    (X,Y,Z) = (raio * sin(theta) * sin(phi), raio * cos(theta), raio * sin(theta) * cos(phi)) (No sistema de eixos do OpenGL)*/
    for(int i = 0; i <= 5; i++){                //Iterar por theta
        float xz = raio * sinf(theta);
        float y = cosf(theta);
        for(int j = 0; j < 10; j++){            //Iterar por phi
            float x = xz * sin(phi);
            float z = xz * cos(phi);
            vCoords[i * 30 + j * 3] = x;                     vCoords[i * 30 + j * 3 + 1] = y;                     vCoords[i * 30 + j * 3 + 2] = z;
            vColors[i * 30 + j * 3] = rand() % 255 / 255.0;  vColors[i * 30 + j * 3 + 1] = rand() % 255 / 255.0;  vColors[i * 30 + j * 3 + 2] = rand() % 255 / 255.0;
            //Textura duma esfera? (U, V) = (phi / 2pi, theta / pi)
            phi += phiStep;
        }
        theta += thetaStep;
        phi = 0.0;
    }
}

void Sphere::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix){
    printf("todo lol.\n");
}