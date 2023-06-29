#version 330 core

layout ( location = 0 ) in vec4 position;
layout ( location = 1 ) in vec2 texCoord;
out vec2 v_texCoord;
void main() {
	gl_Position = position;
	v_texCoord = texCoord;
}