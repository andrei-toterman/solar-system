#version 330

out vec4 color;
in vec2 tc;
uniform sampler2D samp;

void main() {
    color = texture(samp, tc);
}