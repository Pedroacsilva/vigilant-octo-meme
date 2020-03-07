#version 330 core

//INPUT DATA
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

//OUTPUT DATA
out vec3 fragColor;
out vec3 colorChange;

//UNIFORM DATA
uniform mat4 MVPMatrix;
uniform int time;


void main(){
	mat4 SMatrix;			//Declarar matriz de scaling
	SMatrix[0] = vec4(time % 1000 / 1000.0 * 2, 0, 0, 0);
	SMatrix[1] = vec4(0, time % 1000 / 1000.0 * 2, 0, 0);
	SMatrix[2] = vec4(0, 0, time % 1000 / 1000.0 * 2, 0);
	SMatrix[3] = vec4(0, 0, 0, 1);
	gl_Position = MVPMatrix * (vec4(vertexPosition_modelspace, 1) * SMatrix);
	fragColor = vertexColor;
	colorChange = vec3(sin(time/1000.0), sin(time/500.0), sin(time/250.0));
}

