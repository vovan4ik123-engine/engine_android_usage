#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoords;

out vec2 textureCoords;

uniform mat4 VPMatrix;

void main()
{
    textureCoords = inTextureCoords;

    gl_Position = VPMatrix * vec4(inPosition, 1.0f);
}