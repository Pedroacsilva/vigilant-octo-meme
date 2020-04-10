#version 330 core

//INPUT DATA
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColors;
layout(location = 2) in vec2 vertexTexCoords;

//UNIFORM DATA
uniform mat4 MVPMatrix;

//OUTPUT DATA
out vec3 fragColors
out vec2 fragTexCoords;

void main(){

  gl_Position = MVPMatrix * vec4(vertexPosition, 1.0);
  //gl_Position.w = 1.0;
  fragColors = vertexColors;
  fragTexCoords = vertexTexCoords;
  
}