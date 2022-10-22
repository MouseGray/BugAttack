#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texture_coords;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection*vec4(vertex.xy, 0.0f, 1.0f);
    texture_coords = vertex.zw;
} 
