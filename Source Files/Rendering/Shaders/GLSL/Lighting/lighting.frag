#version 460 core

struct DirLight {
	bool is_lighting;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
	bool is_lighting;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 attenuation_params;
};

in vec2 vTexCoord;

out vec4 pixel_color;

#define NR_DIRECTIONAL_LIGHTS 1
uniform DirLight dir_lights[NR_DIRECTIONAL_LIGHTS];
#define NR_POINT_LIGHTS 1
uniform PointLight point_lights[NR_POINT_LIGHTS];

uniform sampler2D g_position;
uniform sampler2D g_albedo_spec;
uniform sampler2D g_normal_shin;

uniform int is_shaded;
uniform vec3 view_pos;

vec3 CalcDirLight(DirLight light, vec3 view_dir, vec3 albedo_material, float specular_material, vec3 normal_material, float shininess_material){
	vec3 dir_light_norm = normalize(-light.direction);

	//Diffuse
	float diff = max(dot(normal_material, dir_light_norm), 0.0);

	//Specular
	float spec = pow(max(dot(normal_material, normalize(dir_light_norm + view_dir)), 0.0), shininess_material);

	//Result
	vec3 ambient = light.ambient * albedo_material;
	vec3 diffuse = light.diffuse * diff * albedo_material;
	vec3 specular = light.specular * spec * specular_material;
	
	return (ambient + diffuse + specular) * is_shaded + albedo_material * (1 - is_shaded);
}

vec3 CalcPointLight(PointLight light, vec3 view_dir, vec3 frag_pos, vec3 albedo_material, float specular_material, vec3 normal_material, float shininess_material){
	vec3 dir_light_norm = normalize(light.position - frag_pos);

	//Diffuse
	float diff = max(dot(normal_material, dir_light_norm), 0.0);

	//Specular
	float spec = pow(max(dot(normal_material, normalize(dir_light_norm + view_dir)), 0.0), shininess_material);

	//Attenuation
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.attenuation_params.x + light.attenuation_params.y * distance + light.attenuation_params.z * (distance * distance));
	
	//Result
	vec3 ambient = light.ambient * albedo_material  * attenuation;
	vec3 diffuse = light.diffuse * diff * albedo_material  * attenuation;
	vec3 specular = light.specular * spec * specular_material  * attenuation;

	return (ambient + diffuse + specular) * is_shaded + albedo_material * (1 - is_shaded);
}

void main(){
	//GBuffer
	vec3 frag_pos = texture(g_position, vTexCoord).rgb;
	vec3 view_dir = normalize(view_pos - frag_pos);
	
	vec4 albedo_spec = texture(g_albedo_spec, vTexCoord).rgba;
	vec3 albedo = albedo_spec.rgb;
	float specular = albedo_spec.a;
	
	vec4 normal_shin = texture(g_normal_shin, vTexCoord).rgba;
	vec3 normal = normal_shin.rgb;
	float shininess = normal_shin.a;
	
	
	vec3 result = vec3(0.0);
	for (int i = 0; i < NR_DIRECTIONAL_LIGHTS; ++i){
		if (dir_lights[i].is_lighting) result += CalcDirLight(dir_lights[i], view_dir, albedo, specular, normal, shininess);
	}
	for (int i = 0; i < NR_POINT_LIGHTS; ++i){
		if (point_lights[i].is_lighting) result += CalcPointLight(point_lights[i], view_dir, frag_pos, albedo, specular, normal, shininess);
	}
    pixel_color = vec4(result, 1.0);
}