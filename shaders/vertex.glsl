#version 330
layout (location = 0) in vec3 position;

uniform mat4 mvp;

out vec4 vertex_color;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    vertex_color = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
