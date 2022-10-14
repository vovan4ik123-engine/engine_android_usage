#version 300 es

precision highp float; // highp mediump lowp

in vec3 textureCoords;

out vec4 outColor;

uniform samplerCube diffuseTexture;

void main()
{
    outColor = texture(diffuseTexture, textureCoords);
}