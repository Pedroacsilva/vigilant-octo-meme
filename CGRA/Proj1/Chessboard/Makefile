#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: chessboard table deecshader



deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

chessboard: chessboard.o deecshader.o 
	g++ -g chessboard.o deecshader.o -L/opt/local/lib -o chessboard -lglut -lGLEW -lGL -lX11

chessboard.o: chessboard.cpp deecshader.h
	g++ -c -g chessboard.cpp

table: table.o deecshader.o
	g++ -g table.o deecshader.o -L/opt/local/lib  -o table -lglut -lGLEW -lGL -lX11

table.o: table.cpp deecshader.h
	g++ -c -g table.cpp

clean:
	rm -f chessboard table deecshader ~.o