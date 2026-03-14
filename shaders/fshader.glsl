#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 Normal;
in vec3 FragPos;

uniform Material material;

uniform vec3 light_color;
//uniform float alpha;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main(void)
{
    vec3 ambient = vec3(0.1) * material.ambient;  // ambient

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(0.5) * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light_color * (spec * material.specular);

    gl_FragColor = vec4((ambient + diffuse + specular), 1.0);
}
