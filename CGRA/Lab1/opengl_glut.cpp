/*COMPUTAÇÃO GRÁFICA E REALIDADE AUMENTADA, 2019/2020
Autores:
-Gabriel Lopes
-Pedro Silva*/

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <GL/glut.h>
#include <stdio.h>

float rotacao = 0.0f;   //Variável global que define o ângulo de rotação. Global porque a preguiça é muita.

/*https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glClearColor.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glClearDepth.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glShadeModel.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLoadIdentity.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml*/

void InitGL(int Width, int Height) {
  glPointSize(4.0);   //Como quero desenhar a origem, acrescentei esta linha.
  glClearColor( 0.5, 0.5, 0.5, 1.0 );     //especificar valores a escrever nos color buffers quando chamamos glClear
  glClearDepth( 1.0 );                    //E quando chamamos glClearDepth (limpar o depth buffer: https://www.opengl.org/archives/resources/faq/technical/depthbuffer.htm)
  glDepthFunc(GL_LESS);                   //Qual o valor a usar nos testes de depth. GL_LESS-> Desenhar se o valor depth é inferior ao valor armazenado no depth buffer nessas coordenadas (ie: pixel esta a frente)
  glEnable(GL_DEPTH_TEST);                //Ligar depth test (Entre vários pixeis, qual o pixel a desenhar, em função das suas coordenadas, tipicamente z)
  glShadeModel(GL_SMOOTH);                //Especificar técnica de shading. GL_FLAT ou GL_SMOOTH. Smooth interpola as cores dos vertices do fragmento, flat utiliza o color value dum vertice para o fragmento todo
  glMatrixMode(GL_PROJECTION);            //Especificar a matriz em que trabalhamos. GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE, GL_COLOR.
  glLoadIdentity();                       //Substituir a matriz actual pela matriz identidade.
  gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);     //Montar a matriz perspectiva/projecção
  glMatrixMode(GL_MODELVIEW);             //Especificar a matriz em que trabalhamos. GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE, GL_COLOR.
  /*Terei de re-estudar isto, mas GL_MODELVIEW, PROJECTION matrixes transformam os vértices para vários espaços.
  Coordenadas em função do centro do modelo---Model Matrix--> Em função do centro do mundo ---View Matrix--> Em função da câmara ---Projection Matrix--> Em função do frustrum da câmara (o que a camara ve). Tudo dentro desse sólido, é visivel e desenhado no ecrã.*/
}
/*https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glViewport.xml*/
void ReSizeGLScene(int Width, int Height){
  glViewport(0, 0, Width, Height);       //Definir o viewport em função das dimensões da janela da aplicação. O viewport é a janela 2D em que a cena 3D será desenhada. Análogo ao filme fotográfico duma câmara
  glMatrixMode(GL_PROJECTION);           //Especificar em que matriz trabalhamos.
  glLoadIdentity();                      //Substituir por matriz identidade.
  gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);   //Montar matriz perspectiva. Câmara -> Frustum da câmara.
  glMatrixMode(GL_MODELVIEW);             //Especificar em que matriz trabalhamos. Tipicamente, nesta aplicação, estaremos em MODELVIEW, mas ao mudar as dimensões da janela, temos de trabalhar na matriz de projecção. Pelo que trocamos para PROJECTION, e depois de ajustarmos, voltamos para MODELVIEW
} 

/*https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClear.xml
https://www.opengl.org/resources/libraries/glut/spec3/node21.html
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glFlush.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColor.xml
*/
void DrawGLScene( void ) {
  glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);     //Limpar buffers de acordo com ClearColor e ClearDepth.
  glLoadIdentity();
  /* YOUR CODE HERE */
  glTranslatef(0.0, 0.0, -6.0);  //Aplicar uma translação a tudo que iremos desenhar.
  glRotatef(rotacao, 0.0, 0.0, 1.0);   //Rodar 'rotação' graus em torno do eixo z

    //--------------------PONTO--------------------------------
  /*Quero desenhar a origem por onde passa o eixo de rotacao.*/
  glBegin(GL_POINTS);
  glColor3f(1.0, 1.0, 0.0);   //Laranja
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
  //-------------------------------TRIANGULO-------------------------------------------------------
    /*Vamos desenhar um triangulo com vértices em (0, 1), (1,-1) e (-1, -1)
        A
      /   \
     /     \
    /       \
   /         \
  B-----------C
  */
  glTranslatef(-2.0, 0.0, 0.0);
  glBegin(GL_POLYGON);          //Começar o desenho dum polígono A

  glColor3f(1.0, 0.0, 0.0);     //Cor A
  glVertex3f(0.0, 1.0, 0.0);    //Coordenadas A
  glColor3f(0.0, 1.0, 0.0);     //Cor B
  glVertex3f(1.0, -1.0, 0.0);   //Coordenadas B
  glColor3f(0.0, 0.0, 1.0);     //Cor C
  glVertex3f(-1.0, -1.0, 0.0);  //Coordenadas C
  glEnd();        //Terminar o desenho do polígono A
  //-------------------------------QUADRADO----------------------------------------------------
  /*Quadrado com vértices em (-1,1), (1,1), (1,-1), (-1,-1)
  A-----------B
  |           |
  |           |
  |           |
  |           |
  D-----------C
*/
  glTranslatef(4.0, 0.0, 0.0);  //Quadrado 4 unidades à direita do triângulo. (Só o quadrado sofre a translação)
  glBegin(GL_QUADS);        //Começar o desenho dum quadrado B

  glVertex3f(-1.0, 1.0, 0.0);   //Coordenadas A
  glVertex3f(1.0, 1.0, 0.0);    //Coordenadas B
  glVertex3f(1.0, -1.0, 0.0);   //Coordenadas C
  glVertex3f(-1.0, -1.0, 0.0);  //Coordenadas D


  glEnd();          //Terminar desenho do quadrado B


  glutSwapBuffers();            //Se estivermos a trabalhar com 2 buffers (double buffered), trocar os buffers. O back buffer se torna undefined (desconhecido). glFlush implicito 
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
/*https://www.opengl.org/resources/libraries/glut/spec3/node20.html
*/
void idle( void ) {
  rotacao += 0.1;        //Incrementar o angulo de rotacao. Como nao estou a usar nenhum mecanismo de tempo, a cada ciclo do mainloop em que estamos idle, a rotacao aumenta. Logo, vai ser descontrolada e dependerá da carga da máquina (e de cada máquina).
  printf("Rotação: %f.\n", rotacao);    //print out do angulo de rotacao.
  glutPostRedisplay();        //Marca que a janela actual deve ser redesenhada. (Porque não chamar esta função no final de cada SwapBuffers, para o buffer ser imediatamente desenhado no ecrã?)
}
/*https://www.opengl.org/resources/libraries/glut/spec3/node12.html -> DisplayMode
https://www.opengl.org/resources/libraries/glut/spec3/node11.html ->WindowSize&Position
https://www.opengl.org/resources/libraries/glut/spec3/node16.html -> CreateWindow
https://www.opengl.org/resources/libraries/glut/spec3/node46.html ->DisplayFunc
https://www.opengl.org/resources/libraries/glut/spec3/node48.html -> ReshapeFunc
https://www.opengl.org/resources/libraries/glut/spec3/node49.html -> KeyboardFunc
https://www.opengl.org/resources/libraries/glut/spec3/node63.html -> IdleFunc
https://www.opengl.org/resources/libraries/glut/spec3/node14.html -> MainLoop
*/
int main( int argc, char** argv ) {
  glutInit(&argc, argv);

  glutInitDisplayMode( GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH );
  glutInitWindowSize(640,480);
  glutInitWindowPosition(0,0);
  glutCreateWindow("CG&RA GLUT");
  glutDisplayFunc( DrawGLScene );  // register callback routines
  glutReshapeFunc( ReSizeGLScene );
  glutKeyboardFunc( keyPressed );
  glutIdleFunc( idle );
  InitGL(640,480); // initiate OpenGL states, program variables
  glutMainLoop(); // enter the event-driven loop
}
