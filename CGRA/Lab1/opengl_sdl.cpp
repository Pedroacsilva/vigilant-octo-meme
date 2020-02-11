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

void InitGL(int Width, int Height) {
    glClearColor( 0.0, 1.0, 0.0, 1.0 );
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
	    }
	}
 
	DrawGLScene();
    } // Main loop
    return 0;
}
