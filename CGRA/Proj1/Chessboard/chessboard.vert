#version 330 core

//INPUT DATA
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

//OUTPUT DATA

out vec2 UV;
void main(){

  gl_Position.xyz = vertexPosition;
  gl_Position.w = 1.0;
  UV = vertexUV;
}
