#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: tablechess deecshader



deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

tablechess: tablechess.o deecshader.h
	g++ -g tablechess.o deecshader.o -L/opt/local/lib -o tablechess -lglut -lGLEW -lGL -lX11

tablechess.o: tablechess.cpp
	g++ -c -g tablechess.cpp

clean:
	rm -f tablechess deecshader ~.o