#include <stdlib.h>
#include <stdio.h>
/* Ensure we are using opengl's core profile only */
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
//#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>
#endif
#include "deecshader.h"
#include <iostream>
#define PROGRAM_NAME "CG&RA 2020"

/**********************/
int numverticestodraw=2;
bool loop=true;

/* A simple function that prints a message, the error code returned by SDL, and quits the application */
void sdldie(char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}



void ReSizeGLScene(int Width, int Height){
    // do all the processing you need for accounting
    // for the window (and viewport) size  change    
}

void DrawGLScene(SDL_Window *window, DEECShader * shaderprog  ) {
  glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

/* YOUR CODE HERE */

   /* Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color */
    /* Attribute locations must be setup before calling glLinkProgram. */
   
    /* Load the shader into the rendering pipeline */
    shaderprog->startUsing();

    /* Loop our display increasing the number of shown vertexes each time.
     * Start with 2 vertexes (a line) and increase to 3 (a triangle) and 4 (a diamond) */
        /* Make our background black */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes */
	if (loop)
	    glDrawArrays(GL_LINE_LOOP, 0, numverticestodraw);
	else
	    glDrawArrays(GL_LINE_STRIP, 0, numverticestodraw);
	
  
	/* Cleanup all the things we bound and allocated */
	shaderprog->stopUsing();
	
	/* Swap our buffers to make our changes visible */
        SDL_GL_SwapWindow(window);

}



void setupwindow(SDL_Window **window, SDL_GLContext *context)
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVERYTHING) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */

    /* Create our window centered at 512x512 resolution */
    *window = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			       512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if (!*window) /* Die if creation failed */
        sdldie("Unable to create window");

    /* Request an opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);



    /* Create our opengl context and attach it to our window */
    *context = SDL_GL_CreateContext(*window);
    if(*context==NULL){
	printf("Problem creating context: %s\n",SDL_GetError());
	abort();
    }
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
#endif
    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

    std::cout << "Vendor: " << glGetString (GL_VENDOR)<< std::endl;
    std::cout << "Renderer: " << glGetString (GL_RENDERER)<< std::endl;
    std::cout << "Version: " << glGetString (GL_VERSION)<< std::endl;
    std::cout << "GLSL: " << glGetString (GL_SHADING_LANGUAGE_VERSION)<< std::endl;
}

GLuint vao, vbo[2];

void createScene()
{
    int i; /* Simple iterator */
   /* Create handles for our Vertex Array Object and two Vertex Buffer Objects */


    /* We're going to create a simple diamond made from lines */
    const GLfloat diamond[4][2] = {
	{  0.0,  1.0  }, /* Top point */
	{  1.0,  0.0  }, /* Right point */
	{  0.0, -1.0  }, /* Bottom point */
	{ -1.0,  0.0  } }; /* Left point */

    const GLfloat colors[4][3] = {
	{  1.0,  0.0,  0.0  }, /* Red */
	{  0.0,  1.0,  0.0  }, /* Green */
	{  0.0,  0.0,  1.0  }, /* Blue */
	{  1.0,  1.0,  1.0  } }; /* White */

    /* These pointers will receive the contents of our shader source code files */



    /* This is a handle to the shader program */
    GLuint shaderprogram;

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);

    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);

    /* Allocate and assign two Vertex Buffer Objects to our handle */
    glGenBuffers(2, vbo);

    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    /* Copy the vertex data from diamond to our buffer */
    /* 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values */
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

    /* Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable attribute index 0 as being used */
    glEnableVertexAttribArray(0);

    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    /* Copy the color data from colors to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable attribute index 1 as being used */
    glEnableVertexAttribArray(1);



}

void destroywindow(SDL_Window *window, SDL_GLContext context)
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/* Our program's entry point */
int main(int argc, char *argv[])
{
    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */

    /* Create our window, opengl context, etc... */
    setupwindow(&mainwindow, &maincontext);
    DEECShader * shaderprog = new DEECShader;


    shaderprog->loadShaders("lines.vert","lines.frag");
    glBindAttribLocation(shaderprog->shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprog->shaderprogram, 1, "in_Color");

    shaderprog->linkShaderProgram();
    createScene();
    bool Running=true;
    bool FullScreen=false;
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
		case SDLK_l:
		    loop=!loop;
		    break;
		case SDLK_SPACE:
		    numverticestodraw=(numverticestodraw+1)%5; // go from 0 to 4
		    break;
		case SDLK_f:
		    FullScreen = !FullScreen;
		    if (FullScreen) {
			SDL_SetWindowFullscreen(mainwindow, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL | 
				       	SDL_WINDOW_FULLSCREEN_DESKTOP);
		    }
		    else {
			SDL_SetWindowFullscreen(mainwindow, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
		    }		      
		    break;
		}
	    }
	}
 
	DrawGLScene(mainwindow,shaderprog);
    } // Main loop

    delete shaderprog;

    /* Delete our opengl context, destroy our window, and shutdown SDL */
    destroywindow(mainwindow, maincontext);
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}
