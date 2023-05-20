#version 450 core
			
layout(location = 0) out vec4 color;

uniform vec3 u_color = vec3(0, 0, 0);
uniform sampler2D u_textureSampler;

in vec2 f_uv;

void main()
{
    color = vec4(u_color * texture2D(u_textureSampler, f_uv).xyz, 1);
}