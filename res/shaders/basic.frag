#version 330 core

varying vec3 color;
uniform vec4 overColor;

void main()
{
    gl_FragColor = vec4(color, 1.0);
}