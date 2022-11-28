#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;

out vec4 col;

void main()
{
    col = inColor;
    gl_Position = inPosition;
}