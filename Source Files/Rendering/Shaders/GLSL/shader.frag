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

struct Material {
    vec3 color;
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 pixel_color;

#define NR_DIRECTIONAL_LIGHTS 1
uniform DirLight dir_lights[NR_DIRECTIONAL_LIGHTS];
#define NR_POINT_LIGHTS 1
uniform PointLight point_lights[NR_POINT_LIGHTS];

uniform int is_shaded;
uniform vec3 view_pos;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 view_dir, vec3 albedo_material, vec3 specular_material, vec3 normal_material){
	vec3 dir_light_norm = normalize(-light.direction);

	//Diffuse
	float diff = max(dot(normal_material, dir_light_norm), 0.0);

	//Specular
	float spec = pow(max(dot(normal_material, normalize(dir_light_norm + view_dir)), 0.0), material.shininess);

	//Result
	vec3 ambient = light.ambient * albedo_material;
	vec3 diffuse = light.diffuse * diff * albedo_material;
	vec3 specular = light.specular * spec * specular_material;
	
	return (ambient + diffuse + specular) * is_shaded + albedo_material * (1 - is_shaded);
}

vec3 CalcPointLight(PointLight light, vec3 view_dir, vec3 albedo_material, vec3 specular_material, vec3 normal_material){
	vec3 dir_light_norm = normalize(light.position - vFragPos);

	//Diffuse
	float diff = max(dot(normal_material, dir_light_norm), 0.0);

	//Specular
	float spec = pow(max(dot(normal_material, normalize(dir_light_norm + view_dir)), 0.0), material.shininess);

	//Attenuation
	float distance = length(light.position - vFragPos);
	float attenuation = 1.0 / (light.attenuation_params.x + light.attenuation_params.y * distance + light.attenuation_params.z * (distance * distance));
	
	//Result
	vec3 ambient = light.ambient * albedo_material  * attenuation;
	vec3 diffuse = light.diffuse * diff * albedo_material  * attenuation;
	vec3 specular = light.specular * spec * specular_material  * attenuation;

	return (ambient + diffuse + specular) * is_shaded + albedo_material * (1 - is_shaded);
}

void main(){
	vec3 view_dir = normalize(view_pos - vFragPos);
	
	vec3 albedo = texture(material.diffuse_map, vTexCoord).rgb * material.color;
	vec3 specular = texture(material.specular_map, vTexCoord).rgb;
	vec3 normal = normalize(vNormal);
	
	vec3 result = vec3(0.0);
	for (int i = 0; i < NR_DIRECTIONAL_LIGHTS; ++i){
		if (dir_lights[i].is_lighting) result += CalcDirLight(dir_lights[i], view_dir, albedo, specular, normal);
	}
	for (int i = 0; i < NR_POINT_LIGHTS; ++i){
		if (point_lights[i].is_lighting) result += CalcPointLight(point_lights[i], view_dir, albedo, specular, normal);
	}
    pixel_color = vec4(result, 1.0);
}