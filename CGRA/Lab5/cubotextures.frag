#version 330 core


//INPUT DATA
in vec2 UV;
in vec3 vertexColor;
//OUTPUT DATA
out vec3  fragColor;
//UNIFORM DATA 
uniform sampler2D textureSampler;




void main(){
  //fragColor = texture(textureSampler, UV) * vertexColor;		//ESTA LINHA DA ERRO (???????????)
  fragColor = texture(textureSampler, UV).rgb;
}

