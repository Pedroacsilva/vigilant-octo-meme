/*COMPUTAÇÃO GRÁFICA E REALIDADE AUMENTADA, 2019/2020
Autores:
-Gabriel Lopes
-Pedro Silva*/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 640
#define WinHeight 480
SDL_Window *Window;


float rotacao = 0.0f;   //Variável global que define o ângulo de rotação. Global porque a preguiça é muita.

void InitGL(int Width, int Height) {
	glPointSize(4.0);   //Como quero desenhar a origem, acrescentei esta linha.
    //glClearColor( 0.0, 1.0, 0.0, 1.0 );			//Limpar color buffer com Verde
    glClearColor(0.2, 0.2, 0.2, 1.0);					//Limpar color buffer com algo menos chocante
    glClearDepth( 1.0 );	
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);                         
    glShadeModel(GL_SMOOTH);                       
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void ReSizeGLScene(int Width, int Height){
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void DrawGLScene( void ) {
  glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); 
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

  SDL_GL_SwapWindow(Window);
}

int main (int ArgCount, char **Args) {

    u32 WindowFlags = SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL;
    Window = SDL_CreateWindow("CG&RA SDL", 0, 0, 
					  WinWidth, WinHeight, WindowFlags);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
  
    b32 Running = 1;
    b32 FullScreen = 0;

    InitGL(WinWidth,WinHeight);
  
    while (Running)  { // Main loop
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
	    switch(ev.type) {
	    case SDL_QUIT:
		Running=false;
		break;
	    case SDL_WINDOWEVENT:
		switch (ev.window.event) {
		case SDL_WINDOWEVENT_RESIZED:
		    SDL_Log("Window %d resized to %dx%d",
			    ev.window.windowID, ev.window.data1,
			    ev.window.data2);
		    ReSizeGLScene(ev.window.data1, ev.window.data2);
		    break;
		} break;
	
	    case SDL_KEYDOWN:
		switch (ev.key.keysym.sym){
		case  SDLK_ESCAPE:
		case SDLK_q:
		    exit(0);
		case SDLK_f:
		    FullScreen = !FullScreen;
		    if (FullScreen) {
			SDL_SetWindowFullscreen(Window, WindowFlags | 
				       	SDL_WINDOW_FULLSCREEN_DESKTOP);
		    }
		    else {
			SDL_SetWindowFullscreen(Window, WindowFlags);
		    }		      
		    break;
		}
		/*default:					//default = idle, penso que é uma aproximiação razoável
			rotacao += 0.1;			//incrementar angulo de rotacao
			printf("Rotacao = %f.\n", rotacao);*/
		//Após correr o programa, o default só corre quando mexo  rato na janela
	    }
	}
 
	DrawGLScene();
	//Idle?
	rotacao += 0.1;
	printf("Rotacao = %f.\n", rotacao);
    } // Main loop
    return 0;
}
