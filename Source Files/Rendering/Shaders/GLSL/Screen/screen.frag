#version 460 core

in vec2 vTexCoord;

out vec4 pixel_color;

uniform sampler2D screen_texture;

void main(){
	pixel_color = texture(screen_texture, vTexCoord);
	//pixel_color = vec4(1.0, 0.0, 0.0, 1.0);
}