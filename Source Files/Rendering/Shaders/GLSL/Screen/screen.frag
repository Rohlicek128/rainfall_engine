#version 460 core

in vec2 vTexCoord;

out vec4 pixel_color;

uniform sampler2D screen_texture;

uniform float gamma;
uniform float exposure;

/*vec3 Tonemap_ACES(vec3 x) {
	// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return (x * (a * x + vec3(b))) / (x * (c * x + vec3(d)) + vec3(e));
}*/

void main(){
	vec3 color = texture(screen_texture, vTexCoord).rgb;
	
	//Exposure Reinhart
	vec3 mapped = vec3(1.0) - exp(-color * exposure);
	
	//vec3 mapped = Tonemap_ACES(color);
	
	//Gamma
	mapped = pow(mapped, vec3(1.0 / gamma));
	
	pixel_color = vec4(mapped, 1.0);
	//pixel_color = vec4(1.0, 0.0, 0.0, 1.0);
}