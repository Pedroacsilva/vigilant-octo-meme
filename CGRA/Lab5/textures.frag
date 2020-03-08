#version 330 core


//INPUT DATA
in vec2 UV;

//OUTPUT DATA
out vec3 color;
//UNIFORM DATA 
uniform sampler2D textureSampler;




void main(){
  color = texture(textureSampler, UV).rgb;
}

