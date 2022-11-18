#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;

uniform mat4 MVPMatrix;

void main()
{
    gl_Position = MVPMatrix * vec4(inPosition, 1.0f);
}