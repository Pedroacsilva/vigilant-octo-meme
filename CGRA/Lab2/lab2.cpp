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



GLUquadric * cylinder;    //Cilindro
GLUquadric * sphere;    //esfera
GLUquadric * disk;    //disk
int newTime, oldTime, deltaTime = 0;             //Variáveis para animar rotações
int newTimeX, newTimeY, newTimeZ = 0;        //Angulo de rotacao sobre cada eixo
bool rot_x, rot_y, rot_z = false;      //Rotações sobre os 3 eixos
bool post_x, post_y, post_z = false;    //Pós-Translações sobre os 3 eixos
bool pre_x, pre_y, pre_z = false;        //Pré translações sobre os 3 eixos
/*https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glClearColor.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glClearDepth.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glShadeModel.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLoadIdentity.xml
https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml*/

void InitGL(int Width, int Height) {
  //Inicializar objectos a desenhar
  cylinder = gluNewQuadric();
  sphere = gluNewQuadric();
  disk = gluNewQuadric();
  glPointSize(4.0);
  glClearColor( 0.5, 0.5, 0.5, 1.0 );     //especificar valores a escrever nos color buffers quando chamamos glClear
  glClearDepth( 1.0 );                    //E quando chamamos glClearDepth (limpar o depth buffer: https://www.opengl.org/archives/resources/faq/technical/depthbuffer.htm)
  glDepthFunc(GL_LESS);                   //Qual o valor a usar nos testes de depth. GL_LESS-> Desenhar se o valor depth é inferior ao valor armazenado no depth buffer nessas coordenadas (ie: pixel esta a frente)
  glEnable(GL_DEPTH_TEST);                //Ligar depth test (Entre vários pixeis, qual o pixel a desenhar, em função das suas coordenadas, tipicamente z)
  glShadeModel(GL_SMOOTH);                //Especificar técnica de shading. GL_FLAT ou GL_SMOOTH. Smooth interpola as cores dos vertices do fragmento, flat utiliza o color value dum vertice para o fragmento todo
  glMatrixMode(GL_PROJECTION);            //Especificar a matriz em que trabalhamos. GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE, GL_COLOR.
  glLoadIdentity();                       //Substituir a matriz actual pela matriz identidade.
  gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);     //Montar a matriz perspectiva/projecção
  glMatrixMode(GL_MODELVIEW);             //Especificar a matriz em que trabalhamos. GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE, GL_COLOR.

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
  //GLUquadric * cylinder = gluNewQuadric();
  glTranslatef(0.0, 0.0, -6.0);
   //------------------------ORIGEM--------------------
  glBegin(GL_POINTS);
  glColor3f(1.0, 1.0, 0.0);   //Amarelo
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
  glTranslatef(3*post_x, 3*post_y, 3*post_z);     //Pós translações
  //glRotatef(0.02 * newTime , rot_x, rot_y, rot_z);    //rot_xyz definem as componentes sobre os eixos xyz. 1 ou 0
  //calcular angulos de rotacoes
  if(rot_x)
    newTimeX += deltaTime;
  if(rot_y)
    newTimeY += deltaTime;
  if(rot_z)
    newTimeZ += deltaTime;
  glRotatef(0.02 * newTimeX, 1.0, 0.0, 0.0);     //Rotacao sobre o eixo x
  glRotatef(0.02 * newTimeY, 0.0, 1.0, 0.0);     //Rotacao sobre o eixo y
  glRotatef(0.02 * newTimeZ, 0.0, 0.0, 1.0);     //Rotacao sobre o eixo z
  glTranslatef(3*pre_x, 3*pre_y, 3*pre_z);
   //------------------------CILINDRO------------------
  gluCylinder(cylinder, 1.0, 1.0, 2.0, 10, 10);
  //------------------------ESFERA (TOPO CILINDRO)------
  glTranslatef(0.0, 0.0, 2.0);        //Passar para o topo do cilindro
  //https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluSphere.xml
  glColor3f(0.0, 0.0, 1.0);       //Azul
  gluSphere(sphere, 1.0, 10, 10);
  //-------------------------DISCO (BASE CILINDRO)--------

  glTranslatef(0.0, 0.0, -2.3);     //Passar para a base do cilindro
  glColor3f(0.0, 1.0, 0.0);       //Verde
  gluCylinder(disk, 1.0, 1.0, 0.3, 10, 10);


  //Parece uma toróide pois não desenhamos os topos. Temos os vértices que formam um cilindro, mas nao ligamos os vertices das bases.
  //De forma mais estupida mas mais percebivel, desenhámos um canneloni
  //O cilindro está directammente à nossa frente, deitado sobre o eixo z, daí so vermos o perfil circular (??? há uma forma mais elegante de dizer isto)
  //(Mas então porquê um raio interior e exterior?)
  //2 graus / segundo = 0.002 graus / milisegundo
  
  glutSwapBuffers();            //Se estivermos a trabalhar com 2 buffers (double buffered), trocar os buffers. O back buffer se torna undefined (desconhecido). glFlush implicito 
}
void keyPressed( unsigned char key, int x, int y ) {
  switch( key ) {
  case 'q':
    post_x = !post_x;
    printf("EVENT: post_x = %i.\n", post_x);
    break;
  case 'w':
    post_y = !post_y;
    printf("EVENT: post_y = %i.\n", post_y);
    break;
  case 'e':
    post_z = !post_z;
    printf("EVENT: post_z = %i.\n", post_z);
    break;
  case 'a':
    pre_x = !pre_x;
    printf("EVENT: pre_x = %i.\n", pre_x);
    break;
  case 's':
    pre_y = !pre_y;
    printf("EVENT: pre_y = %i.\n", pre_y);
    break;
  case 'd':
    pre_z = !pre_z;
    printf("EVENT: pre_z = %i.\n", pre_z);
    break;
  case 'x':
    rot_x = !rot_x;
    printf("EVENT: rot_x = %i.\n", rot_x);
    break;
  case 'y':
    rot_y = !rot_y;
    printf("EVENT: rot_y = %i.\n", rot_y);
    break;
  case 'z':
    rot_z = !rot_z;
    printf("EVENT: rot_z = %i.\n", rot_z);
    break;
  }
}
/*https://www.opengl.org/resources/libraries/glut/spec3/node20.html
*/
void idle( void ) {
  deltaTime = newTime - oldTime;
  oldTime = newTime;
  newTime = glutGet(GLUT_ELAPSED_TIME); //https://www.opengl.org/resources/libraries/glut/spec3/node70.html
  //Number of milliseconds since glutInit called (or first call to glutGet(GLUT_ELAPSED_TIME)).
  printf("\rRunning time: %i ms. DeltaTime = %i.", newTime, deltaTime); 
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
  glutCreateWindow("CGRA Cilindro");
  glutDisplayFunc( DrawGLScene );  // register callback routines
  glutReshapeFunc( ReSizeGLScene );
  glutKeyboardFunc( keyPressed );
  glutIdleFunc( idle );
  InitGL(640,480); // initiate OpenGL states, program variables
  glutMainLoop(); // enter the event-driven loop
}
