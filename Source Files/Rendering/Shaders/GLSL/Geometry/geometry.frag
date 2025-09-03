#version 460 core

struct Material {
	vec3 color;
	sampler2D diffuse_map;
	sampler2D specular_map;
	sampler2D normal_map;
	int has_normal_maps;
	float shininess;
};

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
in mat3 vTBN;

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec4 g_albedo_spec;
layout (location = 2) out vec4 g_normal_shin;

uniform Material material;

void main(){
	g_position = vFragPos;
	
	g_albedo_spec.rgb = texture(material.diffuse_map, vTexCoord).rgb * material.color;
	g_albedo_spec.a = texture(material.specular_map, vTexCoord).r;

	g_normal_shin.rgb = normalize(vTBN * (texture(material.normal_map, vTexCoord).rgb * 2.0 - 1.0)) * material.has_normal_maps + normalize(vNormal) * (1 - material.has_normal_maps);
	g_normal_shin.a = material.shininess;
}