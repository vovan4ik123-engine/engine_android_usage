#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextureCoords;

out vec2 textureCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 MVPMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main()
{
    textureCoords = inTextureCoords;
    normal = normalize(normalMatrix * inNormal);

    fragPos = (modelMatrix * vec4(inPosition, 1.0f)).xyz;

    gl_Position = MVPMatrix * vec4(inPosition, 1.0f);
}