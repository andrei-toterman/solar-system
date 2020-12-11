#version 330
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

uniform mat4 mvp;

out vec2 tc;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    tc = texCoord;
}
