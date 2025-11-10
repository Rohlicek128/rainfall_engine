#version 460 core

in vec2 vTexCoord;

out vec4 pixel_color;

uniform sampler2D screen_texture;

uniform float gamma;
uniform float threshold;
uniform float key_value;

/*vec3 Tonemap_ACES(vec3 x) {
	// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return (x * (a * x + vec3(b))) / (x * (c * x + vec3(d)) + vec3(e));
}*/

float calc_auto_exposure(float t, float k){
	float luminance = dot(textureLod(screen_texture, vec2(0.5), 10.0).rgb, vec3(0.2126, 0.7152, 0.0722));
	return log2(max((k / max(luminance, 0.005)), 0.0001)) - t;
}

void main(){
	vec3 color = texture(screen_texture, vTexCoord).rgb;
	
	//Exposure Reinhart
	//vec3 mapped = vec3(1.0) - exp(-color * luminance);
	vec3 mapped = exp2(calc_auto_exposure(threshold, key_value)) * color;
	
	//Gamma
	mapped = pow(mapped, vec3(1.0 / gamma));
	
	pixel_color = vec4(mapped, 1.0);
}