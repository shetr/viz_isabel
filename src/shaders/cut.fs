#version 450 core
			
layout(location = 0) out vec4 color;

uniform float u_minTemp = 0;
uniform float u_maxTemp = 1;
uniform float u_z = 0;

uniform sampler3D u_textureSampler;

in vec2 f_uv;

void main()
{
    float temp = float(texture(u_textureSampler, vec3(f_uv, u_z)));
    float t = (clamp(temp, u_minTemp, u_maxTemp) - u_minTemp) / (u_maxTemp - u_minTemp);
    color = vec4(t, t, t, 1);
}