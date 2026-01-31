#version 460 core

layout (location = 0) in vec4 vertex;

out vec2 vTexCoord;

void main(){
	vTexCoord = vertex.zw;
	gl_Position = vec4(vertex.xy, 0.0, 1.0);
}