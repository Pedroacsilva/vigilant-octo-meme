#ifndef DEECSHADER
#define DEECSHADER

#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif


class DEECShader{
    public:

    GLuint  vertexshader;
    GLuint  fragmentshader;
    GLuint   shaderprogram;
    GLchar *   vertexsource;
    GLchar *   fragmentsource;
    DEECShader(){}
    ~DEECShader(){cleanup();}
    char * filetobuf(const char * file);
    GLuint  loadShaders(const char * vertex_file_path,const char * fragment_file_path);
    GLuint linkShaderProgram();
    void cleanup();
    inline void startUsing(){  glUseProgram(shaderprogram);}
    inline void stopUsing(){ glUseProgram(0);}
};

#endif
