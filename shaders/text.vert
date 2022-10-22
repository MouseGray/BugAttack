#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texture_coords;

uniform mat4 u_projection;

void main()
{
    mat4 invert = mat4(1.0, 0.0, 0.0, 0.0,
                       0.0,-1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 0.0, 1.0);

    gl_Position = invert*u_projection*vec4(vertex.xy, 0.0, 1.0);
    texture_coords = vertex.zw;
}
