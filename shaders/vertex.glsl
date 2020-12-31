#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_coords;
layout (location = 2) in vec2 _texture_coords;

uniform mat4 mvp;
uniform vec3 isSun;

out vec2 texture_coords;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

uniform vec4 globalAmbient;

uniform vec3 PL_position;
uniform vec4 PL_ambient;
uniform vec4 PL_diffuse;
uniform vec4 PL_specular;

uniform vec4 M_ambient;
uniform vec4 M_diffuse;
uniform vec4 M_specular;
uniform float M_shininess;
//ambient
out vec4 varyingColor;

uniform vec3 lightPosition;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    texture_coords = _texture_coords;

    if(isSun[0]==0)
    {   
        vec3 ambient = ((globalAmbient * M_ambient) + (PL_ambient * M_ambient)).xyz;
        vec4 P = mv_matrix * vec4(position,1.0);
        vec3 N = normalize((norm_matrix * vec4(normal_coords,1.0)).xyz);
        vec3 L = normalize(PL_position - P.xyz);
        vec3 diffuse = PL_diffuse.xyz * M_diffuse.xyz * max(dot(N,L), 0.0);
        varyingColor = vec4((ambient + diffuse), 1.0);
        
    }
    else{
        varyingColor = vec4(-1,-1,-1,-1);
    }
    
}
