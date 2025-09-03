#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;
out mat3 vTBN;

uniform mat4 model;
uniform mat3 inverse_model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vTexCoord = texCoord;
	vFragPos = vec3(model * vec4(position, 1.0));
	vNormal = inverse_model * normal;

	vec3 T = normalize(vec3(inverse_model * tangent));
	vec3 N = normalize(vec3(inverse_model * normal));
	T = normalize(T - dot(T, N) * N);
	vTBN = mat3(T, cross(N, T), N);

	gl_Position = projection * view * model * vec4(position, 1.0);
}