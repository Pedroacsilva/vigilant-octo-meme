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
	gl_Position = MVPMatrix * vec4(vertexPosition_modelspace, 1);
	fragColor = vertexColor;
	//colorChange = vec3((time/100) % 1000 / 1000, (time/50) % 1000 / 1000, (time/25) % 1000 / 1000);
}

