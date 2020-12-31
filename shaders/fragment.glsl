#version 330

out vec4 color;
in vec2 texture_coords;
uniform sampler2D sampler;

in vec4 varyingColor;

uniform vec4 globalAmbient;
uniform vec3 PL_position;
uniform vec4 PL_ambient;
uniform vec4 PL_diffuse;
uniform vec4 PL_specular;
uniform vec4 M_ambient;
uniform vec4 M_diffuse;
uniform vec4 M_specular;
uniform float M_shininess;
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 _isSun;


//in vec2 tc;

void main() {

    if(_isSun[0] == 1){
        color = texture(sampler, texture_coords);
    }
    else{
        // normalize the light, normal, and view vectors:
        vec3 L = normalize(varyingLightDir);
        vec3 N = normalize(varyingNormal);
        vec3 V = normalize(-varyingVertPos);
        // compute light reflection vector with respect to N:
        vec3 R = normalize(reflect(-L, N));
        // get the angle between the light and surface normal:
        float cosTheta = dot(L,N);
        // angle between the view vector and reflected light:
        float cosPhi = dot(V,R);
        // compute ADS contributions (per pixel), and combine to build output color:
        vec4 texColor = texture(sampler, texture_coords);
        vec3 ambient = ((globalAmbient * texColor) + (PL_ambient * texColor)).xyz;
        vec3 diffuse = PL_diffuse.xyz * texColor.xyz * max(cosTheta,0.0);
        //vec3 specular = PL_specular.xyz * texColor.xyz * pow(max(cosPhi,0.0), M_shininess);
        color = vec4((ambient*0.2 + diffuse), 1.0);
    }    
}
