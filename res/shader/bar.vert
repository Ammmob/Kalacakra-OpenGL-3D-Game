#version 330 core

layout (location = 0) in vec4 position;
uniform float u_posx;
uniform float u_rate;

void main() {
	gl_Position = position;
    gl_Position.x = u_posx + (position.x - u_posx) * u_rate;
}