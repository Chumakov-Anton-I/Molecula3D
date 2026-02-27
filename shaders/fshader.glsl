#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 obj_color;
uniform vec3 light_color;
uniform float alpha;
uniform vec3 light_pos;

void main(void)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * light_color;
    gl_FragColor = vec4((ambient + diffuse) * obj_color, alpha);
}
