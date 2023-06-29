#version 330 core

layout ( location = 0 ) in vec4 position;
layout ( location = 1 ) in vec2 texCoord;
out vec2 v_texCoord;
uniform float u_posy;
void main() {
	gl_Position = position;
	gl_Position.y += u_posy;
	v_texCoord = texCoord;
}