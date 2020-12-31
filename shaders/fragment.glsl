#version 330

out vec4 color;
in vec2 texture_coords;
uniform sampler2D sampler;

in vec4 varyingColor;
//in vec2 tc;

void main() {
    
    if(varyingColor!=vec4(-1,-1,-1,-1)) // not sun, se aplica ambiental light pe textura
        color = texture(sampler, texture_coords) * varyingColor;
    else // sun, texutra ramane la fel = luminoasa
        color = texture(sampler, texture_coords);
}
