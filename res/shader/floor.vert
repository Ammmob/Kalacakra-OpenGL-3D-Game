#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * position;
    v_normal = normalize(mat3(transpose(inverse(u_model))) * vec3(0.0f,1.0f,0.0f));
    v_fragPos = vec3(u_model * position);
    v_texCoord = texCoord;
}