#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: landscape



deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

landscape: landscape.o deecshader.o
	g++ -g landscape.o deecshader.o -L/opt/local/lib -o landscape -lglut -lGLEW -lGL -lX11

landscape.o:
	g++ -c -g landscape.cpp

clean:
	rm -f landscape deecshader ~.o