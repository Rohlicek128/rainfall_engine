#version 460 core

struct Material {
	vec3 color;
	sampler2D diffuse_map;
	
	float roughness;
	sampler2D roughness_map;

	float metallic;
	sampler2D metallic_map;
	
	sampler2D normal_map;
	int has_normal_map;
};

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
in mat3 vTBN;

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec4 g_albedo_rough;
layout (location = 2) out vec4 g_normal_metal;

uniform Material material;

void main(){
	g_position = vFragPos;
	
	g_albedo_rough.rgb = texture(material.diffuse_map, vTexCoord).rgb * material.color;
	g_albedo_rough.a = texture(material.roughness_map, vTexCoord).g + material.roughness;

	g_normal_metal.rgb = normalize(vTBN * (texture(material.normal_map, vTexCoord).rgb * 2.0 - 1.0)) * material.has_normal_map + normalize(vNormal) * (1 - material.has_normal_map);
	g_normal_metal.a = texture(material.metallic_map, vTexCoord).b + material.metallic;
}