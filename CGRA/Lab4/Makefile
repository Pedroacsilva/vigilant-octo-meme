all:  lines cubo



lines: lines.o deecshader.o deecshader.h
	g++ -g lines.o deecshader.o  -lSDL2  -lGLEW -lGL  -lX11 -o lines

lines.o: lines.cpp deecshader.h
	g++ -c -g lines.cpp -I/usr/include/SDL2

deecshader.o: deecshader.cpp deecshader.h
	g++ -c -g deecshader.cpp 

cubo: cubo.o deecshader.o
	   g++ -g cubo.o deecshader.o -o cubo   -lglut  -lGLEW -lGL -lX11

cubo.o: cubo.cpp deecshader.h
	     g++ -c -g cubo.cpp

clean:
	rm -f lines cubo *.o *~
