#version 330

uniform vec4 global_ambient;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform float shininess;
uniform bool is_sun;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_texture_coord;
in vec3 light_direction;

uniform sampler2D sampler;

out vec4 color;

void main() {
    vec3 L = normalize(light_direction);
    vec3 N = normalize(fragment_normal);
    vec3 V = normalize(-fragment_position);

    vec3 R = normalize(reflect(-L, N));
    float cos_theta = dot(L, N);
    float cos_phi = dot(V, R);

    vec4 texture_color = texture(sampler, fragment_texture_coord);

    vec3 ambient = ((global_ambient + light_ambient) * texture_color).xyz;
    vec3 diffuse = (light_diffuse * texture_color * max(cos_theta, 0.0)).xyz;
    vec3 specular = (light_specular * texture_color * pow(max(cos_phi, 0.0), shininess)).xyz;

    color = is_sun ? texture_color : vec4(ambient + diffuse + specular, 1.0);
}
