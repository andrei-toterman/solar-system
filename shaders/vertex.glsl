#version 330
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 _texture_coords;

uniform mat4 mvp;

out vec2 texture_coords;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    texture_coords = _texture_coords;
}
