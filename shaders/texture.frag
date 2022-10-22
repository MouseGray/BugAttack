#version 330 core

in vec2 texture_coords;

out vec4 color;

uniform sampler2D u_texture;
uniform float u_transparence;

void main()
{
    vec4 texture = texture(u_texture, texture_coords);
    color = vec4(texture.xyz, texture.w*u_transparence);
}
