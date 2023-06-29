#version 330 core
out vec4 FragColor;

in vec3 v_texCoords;

uniform samplerCube u_skybox;

void main() {    
    FragColor = texture(u_skybox, v_texCoords);
}