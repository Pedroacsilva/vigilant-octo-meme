#version 330 core

//INPUT DATA
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;			//Coordenadas Textura dos vertices

//UNIFORM DATA
uniform mat4 MVPMatrix;
uniform int time;



//OUTPUT DATA
out vec2 UV;
out vec3 fragColor;


void main(){
	mat4 TranslationMatrix;			//Matriz de translacao
	TranslationMatrix[0] = vec4(1, 0, 0, 0);
	TranslationMatrix[1] = vec4(0, 1, 0, 0);
	TranslationMatrix[2] = vec4(0, 0, 1, 3 * sin(time / 1000.0));
	TranslationMatrix[3] = vec4(0, 0, 0, 1);
	//gl_Position = TranslationMatrix * MVPMatrix * vec4(vertexPosition_modelspace, 1);
	gl_Position = MVPMatrix * (vec4(vertexPosition_modelspace, 1) * TranslationMatrix);
	UV = vertexUV;			//Passar para frag shader
	fragColor = vertexColor;

}

