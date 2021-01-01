#version 330
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texture_coord;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 normal_matrix;
uniform vec3 light_position;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec2 fragment_texture_coord;
out vec3 light_direction;

void main() {
    gl_Position = mvp * vec4(vertex_position, 1.0);

    fragment_position = (mv * vec4(vertex_position, 1.0)).xyz;
    fragment_normal = (normal_matrix * vec4(vertex_normal, 1.0)).xyz;
    fragment_texture_coord = vertex_texture_coord;
    light_direction = light_position - fragment_position;
}
