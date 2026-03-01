#version 330 core

uniform vec3 obj_color;

void main(void)
{
    gl_FragColor = vec4(obj_color, 1.0);
}
