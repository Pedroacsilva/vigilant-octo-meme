#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
  uniform float timepar;
void main(){

  gl_Position.xyz = vertexPosition_modelspace;
  gl_Position.y = vertexPosition_modelspace.y + sin(timepar/100000 * 360);
  gl_Position.z=0.0;
  gl_Position.w = 1.0;
}

