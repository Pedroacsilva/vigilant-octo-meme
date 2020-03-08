all: objs test textures cubo
# you should add your files to this makefile too

objs: cgraimage.o

cgraimage.o: cgraimage.cpp cgraimage.h
	g++ -g -c -I/opt/local/include cgraimage.cpp

test: 	test.cpp cgraimage.o
	g++ -g test.cpp cgraimage.o -L/opt/local/lib -lnetpbm -o test

deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

textures: textures.o deecshader.o cgraimage.o
	   g++ -g textures.o deecshader.o cgraimage.o -L/opt/local/lib -o textures   -lglut  -lGLEW -lGL -lX11 -lnetpbm

textures.o: textures.cpp deecshader.h cgraimage.h
	     g++ -c -g textures.cpp

cubo: cubo.o deecshader.o cgraimage.o
	   g++ -g cubo.o deecshader.o cgraimage.o -L/opt/local/lib -o cubo -lglut  -lGLEW -lGL -lX11 -lnetpbm

cubo.o: cubo.cpp deecshader.h cgraimage.h
	g++ -c -g cubo.cpp

clean:
	rm -f test  textures *.o *~
