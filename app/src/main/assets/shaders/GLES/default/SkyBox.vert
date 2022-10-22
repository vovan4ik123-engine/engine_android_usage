#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;

out vec3 textureCoords;

uniform mat4 VPMatrix;

void main()
{
    textureCoords = inPosition;

    vec4 pos = VPMatrix * vec4(inPosition, 1.0f);
    gl_Position = pos.xyww;
}