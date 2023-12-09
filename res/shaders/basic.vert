 #version 330 core

attribute vec2 vPos;
attribute vec3 vCol;
uniform vec3 prim_pos;
varying vec3 color;

void main()
{
    vec3 pos = vec3(vPos, 0.0) + prim_pos;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    color = vCol;
}