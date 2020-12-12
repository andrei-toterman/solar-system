#version 330

out vec4 color;
in vec2 texture_coords;
uniform sampler2D sampler;

void main() {
    color = texture(sampler, texture_coords);
}
