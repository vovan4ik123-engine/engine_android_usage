#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;

out vec4 outColor;

uniform sampler2D diffuseTexture;

void main()
{
    outColor = texture(diffuseTexture, textureCoords);
}
