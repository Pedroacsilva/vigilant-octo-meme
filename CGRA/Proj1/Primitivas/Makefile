#Makefile para Proj 1
#AUTORES:
#Pedro Silva
all: teste obj.o deecshader.o cgraimage.o


cgraimage.o: cgraimage.cpp cgraimage.h
	g++ -g -c -I/opt/local/include cgraimage.cpp


obj.o: obj.cpp
	g++ -c -g obj.cpp

deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

teste: teste.o deecshader.o obj.o cgraimage.o
	g++ -g teste.o deecshader.o obj.o  cgraimage.o -o teste -lglut -lGLEW -lGL -lX11 -lnetpbm

teste.o: teste.cpp deecshader.h obj.h cgraimage.h
	g++ -c -g teste.cpp

clean:
	rm -f teste  obj ~.o