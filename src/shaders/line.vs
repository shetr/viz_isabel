#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in float a_vel;
layout(location = 2) in float a_normal;

out float f_vel;

uniform mat4 u_PVM;

void main()
{
    f_vel = a_vel;
    gl_Position = u_PVM * vec4(a_position, 1);
}