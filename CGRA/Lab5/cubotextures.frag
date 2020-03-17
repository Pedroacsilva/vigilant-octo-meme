#version 330 core


//INPUT DATA
in vec2 UV;
in vec3 fragColor;
//OUTPUT DATA
out vec3  color;
//UNIFORM DATA 
uniform sampler2D textureSampler;




void main(){
  color = texture(textureSampler, UV).rgb * fragColor.rgb;		//ESTA LINHA DA ERRO
  //fragColor = texture(textureSampler, UV).rgb;
}

