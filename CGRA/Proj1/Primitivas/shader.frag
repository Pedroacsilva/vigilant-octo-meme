#version 330 core

//INPUT DATA
in vec3 fragColors;
in vec2 fragTexCoords;


//TEXTURE SAMPLER
uniform sampler2D textureSampler;

//OUTPUT DATA
out vec3 color;


void main(){
	color.rgb = texture(textureSampler, fragTexCoords).rgb * fragColors.rgb;
  //color.rgb = min((texture(textureSampler, fragTexCoords).rgb * fragColor), vec3(1.0));
//  color.rgb = vec3(1,0,0);
}
