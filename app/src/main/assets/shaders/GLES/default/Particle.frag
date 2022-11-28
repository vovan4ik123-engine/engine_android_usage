#version 300 es

precision highp float; // highp mediump lowp

out vec4 outColor;

in vec4 col;

void main()
{
    outColor = col;
}