#version 450 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_PVM;

void main()
{
    gl_Position = u_PVM * vec4(a_position, 1);
}