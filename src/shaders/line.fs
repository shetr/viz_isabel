#version 450 core
			
layout(location = 0) out vec4 color;

uniform float u_minVelocity = 0;
uniform float u_maxVelocity = 0;

uniform vec3 u_highColor = vec3(1.0, 0.0, 0.0);
uniform vec3 u_lowColor = vec3(0.0, 1.0, 0.0);

uniform vec3 u_cameraPosition = vec3(0);
uniform vec3 u_lightDir = vec3(0.2, -1.0, 0.2);
uniform float u_shinines = 1.0;

in float f_vel;
in vec3 f_position;
in vec3 f_normal;

void main()
{
    vec3 dirToLight = -u_lightDir;
    vec3 dirToCam = normalize(u_cameraPosition - f_position);
    vec3 reflected = reflect(-dirToLight, f_normal);
    float specularExp = pow(max(dot(dirToCam, reflected), 0), u_shinines);
    if(dot(reflected, f_normal) < 0) {
        specularExp = 0;
    }
    vec3 diffuse = vec3(max(dot(dirToLight, f_normal), 0.5));
    vec3 light = diffuse + specularExp;

    float t = (f_vel - u_minVelocity) / (u_maxVelocity - u_minVelocity);
    vec3 c = (1 - t) * u_lowColor + t * u_highColor;
    color = vec4(c * light, 1.0);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}