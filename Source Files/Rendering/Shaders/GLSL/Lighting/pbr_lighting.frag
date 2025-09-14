#version 460 core

struct DirLight {
	bool is_lighting;
	vec3 direction;
    
	vec3 color;
};

struct PointLight {
	bool is_lighting;
	vec3 position;
	vec3 color;

	vec3 attenuation_params;
};

in vec2 vTexCoord;

out vec4 pixel_color;

#define NR_DIRECTIONAL_LIGHTS 1
uniform DirLight dir_lights[NR_DIRECTIONAL_LIGHTS];
#define NR_POINT_LIGHTS 5
uniform PointLight point_lights[NR_POINT_LIGHTS];

uniform sampler2D g_position;
uniform sampler2D g_albedo_rough;
uniform sampler2D g_normal_metal;

uniform vec3 view_pos;
uniform vec3 ambient;

const float PI = 3.14159265359;

vec3 fresnel_schlick(float cos_theta, vec3 F0){
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cos_theta, 0.0, 1.0), 5.0);
}

float distribution_ggx(vec3 N, vec3 H, float roughness){
	float a2 = pow(roughness, 4.0);
	float N_dot_H = max(dot(N, H), 0.0);
	float N_dot_H2 = N_dot_H * N_dot_H;
	
	float denom = (N_dot_H2 * (a2 - 1.0) + 1.0);
	
	return a2 / (PI * denom * denom);
}

float geometry_schlick_ggx(float n_dot_v, float roughness){
	float k = ((roughness + 1.0) * (roughness + 1.0)) / 8.0;
	return n_dot_v / (n_dot_v * (1.0 - k) + k);
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness){
	return geometry_schlick_ggx(max(dot(N, V), 0.0), roughness) * geometry_schlick_ggx(max(dot(N, L), 0.0), roughness);
}

void main(){
	//GBuffer
	vec3 frag_pos = texture(g_position, vTexCoord).rgb;
	vec3 V = normalize(view_pos - frag_pos);

	vec4 albedo_rough = texture(g_albedo_rough, vTexCoord).rgba;
	vec3 albedo = albedo_rough.rgb;
	float roughness = albedo_rough.a;

	vec4 normal_metal = texture(g_normal_metal, vTexCoord).rgba;
	vec3 N = normal_metal.rgb;
	float metallic = normal_metal.a;

	//PBR LIGHTING
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	vec3 Lo = vec3(0.0);
	
	//Directional Lights
	for (int i = 0; i < NR_DIRECTIONAL_LIGHTS; ++i){
		if (!dir_lights[i].is_lighting) continue;

		vec3 L = normalize(-dir_lights[i].direction);
		vec3 H = normalize(V + L);
		vec3 radiance = dir_lights[i].color * max(dot(N, L), 0.0);

		vec3 F = fresnel_schlick(max(dot(H, V), 0.0), F0);
		float NDF = distribution_ggx(N, H, roughness);
		float G = geometry_smith(N, V, L, roughness);

		vec3 specular = (F * NDF * G) / max(4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0), 0.01);
		vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

		Lo += (kD * albedo / PI + specular) * radiance * max(dot(N, L), 0.0);
	}
	//Point Lights
	for (int i = 0; i < NR_POINT_LIGHTS; ++i){
		if (!point_lights[i].is_lighting) continue;
		
		vec3 L = normalize(point_lights[i].position - frag_pos);
		vec3 H = normalize(V + L);
		
		float distance = length(point_lights[i].position - frag_pos);
		float attenuation = 1.0 / (point_lights[i].attenuation_params.x + point_lights[i].attenuation_params.y * distance + point_lights[i].attenuation_params.z * (distance * distance));
		vec3 radiance = point_lights[i].color * attenuation;
		
		vec3 F = fresnel_schlick(max(dot(H, V), 0.0), F0);
		float NDF = distribution_ggx(N, H, roughness);
		float G = geometry_smith(N, V, L, roughness);
		
		vec3 specular = (F * NDF * G) / max(4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0), 0.01);
		vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
		
		Lo += (kD * albedo / PI + specular) * radiance * max(dot(N, L), 0.0);
	}
	
	//length(frag_pos - view_pos) depth
    
	pixel_color = vec4(((ambient * albedo) + Lo), 1.0);
}