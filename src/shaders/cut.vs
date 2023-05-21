#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

out vec2 f_uv;

uniform mat4 u_PVM;

void main()
{
    f_uv = a_uv;
    gl_Position = vec4(a_position, 1.0);
    //gl_Position = u_PVM * vec4(a_position, 1);
}