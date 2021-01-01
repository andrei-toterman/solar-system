#version 330

in vec2 fragment_texture_coord;

uniform sampler2D sampler;

out vec4 color;

void main() {
    color = texture(sampler, fragment_texture_coord);
}
