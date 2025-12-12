#version 460 core

in vec3 vTexCoord;

out vec4 pixel_color;

uniform samplerCube skybox;
uniform float sun_intensity;

void main(){
	pixel_color = vec4(texture(skybox, vTexCoord).rgb * sun_intensity, 1.0);
	//pixel_color = vec4(1.0, 0.0, 0.0, 1.0);
}