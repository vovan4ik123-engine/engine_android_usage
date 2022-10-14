#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMapTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D heightMapTexture;

void main()
{
    outColor = texture(diffuseTexture, textureCoords);
}