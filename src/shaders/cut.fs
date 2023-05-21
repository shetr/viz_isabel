#version 450 core
			
layout(location = 0) out vec4 color;

uniform float u_minTemp = 0;
uniform float u_maxTemp = 1;
uniform float u_z = 0;

uniform sampler3D u_textureSampler;

in vec2 f_uv;

void main()
{
    vec3 colors[5] = {
        vec3(253, 231, 37) / 255.0,
        vec3(94, 201, 98) / 255.0,
        vec3(33, 145, 140) / 255.0,
        vec3(59, 82, 139) / 255.0,
        vec3(68, 1, 84) / 255.0
    };
    vec3 c1 = vec3(0, 0, 1);
    vec3 c2 = vec3(1, 0, 0);
    float temp = float(texture(u_textureSampler, vec3(f_uv, u_z)));
    float t = (clamp(temp, u_minTemp, u_maxTemp) - u_minTemp) / (u_maxTemp - u_minTemp);
    //vec3 c = vec3(0.0);
    //for (int i = 0; i < 4; ++i) {
    //    float start = float(i) * (1.0 / 5.0);
    //    float end = float(i + 1) * (1.0 / 5.0);
    //    if (t >= start && t < end) {
    //        float t2 = (t - start) / (end - start);
    //        c = (1 - t2) * colors[i] + t2 * colors[i + 1];
    //    }
    //}
    //vec3 c = (1 - t) * c1 + t * c2;
    vec3 c = vec3(t);
    color = vec4(c, 1);
}