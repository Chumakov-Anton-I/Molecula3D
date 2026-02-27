#version 330 core

attribute vec3 vert_pos;
attribute vec3 vert_norm;

uniform mat4 modelMatr;
uniform mat4 viewMatr;
uniform mat4 projMatr;

out vec3 Normal;
out vec3 FragPos;

void main(void)
{
    gl_Position = projMatr * viewMatr * modelMatr * vec4(vert_pos, 1.0);
    Normal = vert_norm; //normalize(mat3(modelMatr) * vert_norm);
    FragPos = vec3(modelMatr * vec4(vert_pos, 1.0));
}
