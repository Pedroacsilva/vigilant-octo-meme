#include"obj.h"



//Implementar Constructor
Square::Square(){
    printf("SQUARE CONSTRUCTOR.\n");
    NumVertices = 4;
    printf("NumVertices inicializado. NumVertices = %i\n", NumVertices);
    //Alocar memória
    printf("Tentar alocar memoria para coordenadas.\n");
    vCoords = new GLfloat[NumVertices * 3];		//Coordenadas
    printf("Alocação de memoria para coordenadas com sucesso.\n");
    vColors = new GLfloat[NumVertices * 3];		//Cores
    printf("Alocação de memoria para cores com sucesso.\n");
    vTexCoords = new GLfloat[NumVertices * 2];	//Texturas
    printf("Alocação de memoria para coordenadas textura com sucesso.\n");
    vEBO = new GLuint[NumVertices];
    printf("Alocação para EBO com sucesso.\n");
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
//Getters
/*GLfloat Square::*getvCoords(){
	return(vCoords);
}*/
//Implementar Square::drawShape()
void Square::drawShape(DEECShader * shaderProg){
	//Gerar e bind VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Gerar VBOs
	unsigned int vboCoords, vboColors, vboTexCoords, vboEBO;
	glGenBuffers(1, &vboCoords);
	glGenBuffers(1, &vboColors);
	glGenBuffers(1, &vboTexCoords);
	glGenBuffers(1, &vboEBO);


	shaderProg->startUsing();
	//Encher buffers + definir layout dos dados 
    glBindBuffer(GL_ARRAY_BUFFER, vboCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vCoords), vCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vColors), vColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoords), vTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(2);
    

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vEBO), vEBO, GL_STATIC_DRAW);

	printf("Print out de dados porque estou desesperado:\nCoordenadas:\n");
    for(int i = 0; i < 12; i+=3)
    	printf("vCoords[%i] = (%f, %f, %f)\n", i/3, vCoords[i], vCoords[i+1], vCoords[i+2]);
    printf("Cores:\n");
    for(int i = 0; i < 12; i+=3)
        printf("vColors[%i] = (%f, %f, %f)\n", i/3, vColors[i], vColors[i+1], vColors[i+2]);
    printf("TexCoords:\n");
    for(int i = 0; i < 8; i+=2)
        printf("vTexCoords[%i] = (%f, %f)\n", i/2, vTexCoords[i], vTexCoords[i+1]);
    printf("Ordem:\n");
    for(int i = 0; i < 6; i++)
        printf("EBO[%i]: %u\n", i, vEBO[i]);



   	//Desenhar triangulos na ordem do EBO
   	printf("Mandar desenhar:\n");
//   	glBindVertexArray(vao);
   	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    shaderProg->stopUsing();
    glFlush();

}