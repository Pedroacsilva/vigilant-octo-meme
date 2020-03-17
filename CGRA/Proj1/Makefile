#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: chessboard



deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

chessboard: chessboard.o deecshader.o 
	g++ -g chessboard.o deecshader.o -o chessboard -lglut -lGLEW -lGL -lX11

chessboard.o: chessboard.cpp deecshader.h
	g++ -c -g chessboard.cpp

clean:
	rm -f chessboard ~.o