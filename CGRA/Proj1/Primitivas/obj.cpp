#include"obj.h"



//Implementar Constructor
Square::Square(){
    printf("Square constructor called.\n");
    NumVertices = 4;
    //Alocar memória
    vCoords = new GLfloat[NumVertices * 3];		//Coordenadas
    vColors = new GLfloat[NumVertices * 3];		//Cores
    vTexCoords = new GLfloat[NumVertices * 2];	//Texturas
    vEBO = new GLuint[NumVertices];
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
}
//Destructor
Square::~Square(){
    delete vCoords;
    delete vColors;
    delete vTexCoords;
    delete vEBO;
    printf("Square destructor called.\n");
}
//Getters
/*GLfloat Square::*getvCoords(){
	return(vCoords);
}*/
//Implementar Square::drawShape()
void Square::drawShape(DEECShader * shaderProg, glm::mat4 MVPMatrix, char * textureName){
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
    //Carregar textura
    CGRAimage textureImage;             //imagem para textura
    printf("Textura a usar: %s\n", textureName);
    if(!textureImage.loadPPM(textureName))
        printf("Error loading image.\n");
    else
        printf("Image loaded successfully.\n");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width, textureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);    
	shaderProg->startUsing();
    //Localizar variável uniforme
    GLuint MVPID = glGetUniformLocation(shaderProg->shaderprogram, "MVPMatrix");
    printf("MVPID: %i\n", MVPID);
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