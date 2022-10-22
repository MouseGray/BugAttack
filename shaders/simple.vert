#version 330 core
layout (location = 0) in vec2 vertex;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection*vec4(vertex.xy, 0.0f, 1.0f);
} 
