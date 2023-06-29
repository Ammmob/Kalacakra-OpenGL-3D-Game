#version 330 core
layout (location = 0) in vec3 position;

out vec3 v_texCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    v_texCoords = position;
    vec4 pos = u_projection * u_view * vec4(position, 1.0);
    gl_Position = pos.xyww;
}