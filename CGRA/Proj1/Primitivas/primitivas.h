/****************************************************************
Conjunto de classes para organizar modelos OpenGL:
- Quadrado
- Cubo
- Cone
- Cilindro
*****************************************************************/


class square{
public:
	//constructor & destructor
	//quadrado com lado 1 por defeito
	square(){}
	~square(){}
	//getters & setters
	float getSide(){
		return(x);
	}
	float setSide(float side);
	//Funções públicas: para desenhar
	void draw();



private:
	float side;
	float r;
	float g;
	float b;
	float * vertices;
	float * texCoords;
	unsigned int * indices;
}