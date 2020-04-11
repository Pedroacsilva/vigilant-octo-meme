#version 330 core

//INPUT DATA
in vec3 fragColor;
in vec2 fragTexCoords;

//OUTPUT DATA

out vec3 color;


void main(){
  //color.rgb = min((texture(textureSampler, fragTexCoords).rgb * fragColor), vec3(1.0));
  color.rgb = vec3(1,0,0);
}
