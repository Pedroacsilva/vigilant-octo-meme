#version 330 core
uniform vec3 color_change;
out vec3 color;
void main(){
  color.x = color_change.x * 0.9;
  color.y = color_change.y * 0.8;
  color.z = color_change.z * 0.7;
}

