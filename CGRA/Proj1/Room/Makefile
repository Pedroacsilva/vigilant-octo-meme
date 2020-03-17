#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: room deecshader



deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

room: room.o deecshader.o
	g++ -g room.o deecshader.o -L/opt/local/lib -o room -lglut -lGLEW -lGL -lX11

room.o: room.cpp deecshader.h
	g++ -c -g room.cpp

clean:
	rm -f chessboard table room deecshader ~.o