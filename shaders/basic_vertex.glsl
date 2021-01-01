#version 330
layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec2 vertex_texture_coord;

uniform mat4 mvp;

out vec2 fragment_texture_coord;

void main() {
    gl_Position = mvp * vec4(vertex_position, 1.0);
    fragment_texture_coord = vertex_texture_coord;
}
