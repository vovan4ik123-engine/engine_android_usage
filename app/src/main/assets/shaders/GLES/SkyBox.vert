#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;

out vec3 textureCoords;

uniform mat4 VP_matrix;

void main()
{
    textureCoords = inPosition;

    vec4 pos = VP_matrix * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
}