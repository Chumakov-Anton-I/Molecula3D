#version 330 core

attribute vec3 vert_pos;

uniform mat4 viewMatr;
uniform mat4 projMatr;

void main(void)
{
    gl_Position = projMatr * viewMatr * vec4(vert_pos, 1.0);
}
