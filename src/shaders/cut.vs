#version 450 core
			
layout(location = 0) in vec3 a_position;

out vec2 f_uv;

void main()
{
    f_uv = a_position.xy;
    gl_Position = vec4(a_position, 1.0);	
}