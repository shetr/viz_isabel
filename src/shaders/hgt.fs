#version 450 core
			
layout(location = 0) out vec4 color;

uniform vec3 u_color = vec3(0.5);
uniform vec3 u_cameraPosition = vec3(0);
uniform vec3 u_lightDir = vec3(0.2, -1.0, 0.2);
uniform float u_shinines = 1.0;

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

    vec3 c = u_color;
    if (f_position.y + 0.5 <= 1.0)
    {
        c = vec3(0.9,0.9,0.95); 
    }
    if (f_position.y + 0.5 <= 0.05)
    {
        c = vec3(0.0,0.6,0.0);
    }
    if (f_position.y + 0.5 <= 0.002)
    {
        c = vec3(0.8,0.6,0.4);
    }
    if (f_position.y + 0.5 <= 0.001)
    {
        c = vec3(0.0,0.0,0.5);
    }
    color = vec4(c * light, 1.0);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}