#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_coords;
layout (location = 2) in vec2 _texture_coords;

uniform mat4 mvp;
uniform vec3 isSun;

out vec2 texture_coords;

uniform mat4 mv_matrix;
uniform mat4 norm_matrix;
uniform vec3 PL_position;

out vec3 _isSun;
out vec3 varyingNormal; // eye-space vertex normal
out vec3 varyingLightDir;// vector pointing to the light
out vec3 varyingVertPos;// vertex position in eye space

void main() {
    _isSun = isSun;
    texture_coords = _texture_coords;

    varyingVertPos=(mv_matrix * vec4(position,1.0)).xyz;
    varyingLightDir = PL_position - varyingVertPos;
    varyingNormal=(norm_matrix * vec4(normal_coords,1.0)).xyz;
    gl_Position = mvp * vec4(position, 1.0);    
}
