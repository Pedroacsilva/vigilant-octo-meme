all: opengl_sdl opengl_glut

opengl_sdl: opengl_sdl.cpp
	g++ opengl_sdl.cpp -lSDL2 -lGLU -lGL -o opengl_sdl

opengl_glut: opengl_glut.cpp
	g++ -o opengl_glut opengl_glut.cpp -lGLU -lglut -lGL

clean:
	rm -f opengl_sdl opengl_glut *~
