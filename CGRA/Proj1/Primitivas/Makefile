#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: teste obj.o deecshader.o

obj.o: obj.cpp
	g++ -c -g obj.cpp

deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

teste: teste.o deecshader.o obj.o
	g++ -g teste.o deecshader.o obj.o  -o teste -lglut -lGLEW -lGL -lX11

teste.o: teste.cpp deecshader.h
	g++ -c -g teste.cpp

clean:
	rm -f teste deecshader obj ~.o