#version 330 core
//INPUT DATA
in vec3 fragColor;
in vec3 colorChange;

//OUTPUT DATA
out vec3 color;

void main(){
  //color = fragColor * colorChange;
  color = fragColor + colorChange;
}

