/*CGRA 2019_2020
Projecto 1: Primitivas
Autor: 
Gabriel Lopes
Pedro Silva (2011149228)*/
/*Objectivo: Agrupar todo o trabalho implementado (chessboard e table por enquanto) num ficheiro
por forma a organizar o projecto*/

square::square(float side){
	side = 1.0;
	r = 1.0;
	g = 1.0;
	b = 1.0;

	vertices[] = {
	side/2.0f, side/2.0f, 0.0f,		//Top Right
	side/2.0f, -side/2.0f, 0.0f, 		//Bottom right
	-side/2.0f, -side/2.0f, 0.0f,		//Bottom Left
	-side/2.0f, side/2.0f, 0.0f		//Top Left
	}

	texCoords[] = {}
