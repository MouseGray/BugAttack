#version 330 core

in vec2 texture_coords;

out vec4 color;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main()
{
    color = vec4(u_color, texture(u_texture, texture_coords).r);
}
