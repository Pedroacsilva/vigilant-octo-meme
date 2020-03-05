#version 330 core

//INPUT DATA
layout(location = 0) in vec3 vertexPosition_modelspace;

//UNIFORM DATA
uniform mat4 MVPMatrix;

void main(){
	gl_Position = MVPMatrix * vec4(vertexPosition_modelspace, 1);
}

