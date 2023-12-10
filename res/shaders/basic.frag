#version 330 core

in vec3 color;
uniform vec3 overColor;

void main()
{
    vec3 blendColor = mix(color, overColor, 0.5);
    gl_FragColor = vec4(blendColor, 1.0);
}