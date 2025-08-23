#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 model;
uniform mat3 inverse_model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vTexCoord = texCoord;
    vNormal = inverse_model * normal;
    vFragPos = vec3(model * vec4(position, 1.0));
    
    gl_Position = projection * view * model * vec4(position, 1.0);
}