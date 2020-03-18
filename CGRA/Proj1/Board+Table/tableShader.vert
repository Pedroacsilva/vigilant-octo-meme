#version 330 core
//INPUT DATA
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColors;

//OUTPUT DATA
out vec3 fragColor;

//UNIFORM DATA
uniform mat4 MVPMatrix;

void main(){
  gl_Position = MVPMatrix * vec4(vertexPosition, 1.0);
  fragColor = vertexColors;
}
