#version 450 core
			
layout(location = 0) out vec4 color;

uniform float u_minTemp = 0;
uniform float u_maxTemp = 1;
uniform float u_shift = 0;
uniform int u_axis = 0;
uniform vec3 u_coldColor = vec3(0.0, 0.0, 1.0);
uniform vec3 u_zeroColor = vec3(1.0);
uniform vec3 u_warmColor = vec3(1.0, 0.0, 0.0);
uniform vec3 u_invalidColor = vec3(0.5);

uniform sampler3D u_textureSampler;

in vec2 f_uv;

void main()
{
    float invalidTempLimit = 1.0e34;
    vec3 colors[5] = {
        vec3(253, 231, 37) / 255.0,
        vec3(94, 201, 98) / 255.0,
        vec3(33, 145, 140) / 255.0,
        vec3(59, 82, 139) / 255.0,
        vec3(68, 1, 84) / 255.0
    };
    vec3 c1 = vec3(0, 0, 1);
    vec3 c2 = vec3(1, 0, 0);
    vec3 texCoord = vec3(0, 0, 0);
    if (u_axis == 0) {
        texCoord = vec3(f_uv.x, f_uv.y, u_shift);
    } else {
        texCoord = vec3(f_uv.x, u_shift, f_uv.y);
    }
    float temp = float(texture(u_textureSampler, texCoord));
    if (temp > invalidTempLimit) {
        color = vec4(u_invalidColor, 1);
    } else {
        bool isFreezing = temp <= 0;
        float maxVal = u_maxTemp;
        if (isFreezing) {
            maxVal = -u_minTemp;
            temp += maxVal;
        }
        float t = temp / maxVal;
        vec3 c = vec3(0.0);
        if (isFreezing) {
            c = (1 - t) * u_coldColor + t * u_zeroColor;
        } else {
            c = (1 - t) * u_zeroColor + t * u_warmColor;
        }
        color = vec4(c, 1);
    }
}