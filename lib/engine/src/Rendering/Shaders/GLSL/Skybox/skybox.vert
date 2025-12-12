#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 vTexCoord;

uniform mat4 view;
uniform mat4 projection;

void main(){
	vTexCoord = position;
	vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
}