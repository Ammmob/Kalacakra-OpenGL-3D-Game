#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoord;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform bool isPlay = false;
void main() {
    if(isPlay){
        vec4 totalPosition = vec4(0.0f);
        vec3 totalNormal = vec3(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(boneIds[i] == -1) 
                continue;
            if(boneIds[i] >=MAX_BONES) {
                totalPosition = position;
                break;
            }
            vec4 localPosition = finalBonesMatrices[boneIds[i]] * position;
            totalPosition += localPosition * weights[i];
            vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
            totalNormal += localNormal * weights[i];
        }
        gl_Position = u_projection * u_view * u_model * totalPosition;
        v_normal = normalize(mat3(transpose(inverse(u_model))) * totalNormal);
        v_fragPos = vec3(u_model * totalPosition);
    }else{
        gl_Position = u_projection * u_view * u_model * position;
        v_normal = normalize(mat3(transpose(inverse(u_model))) * normal);
        v_fragPos = vec3(u_model * position);
    }
    v_texCoord = texCoord;
}
