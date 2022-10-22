#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;

out vec4 outColor;

uniform sampler2D shadowMapTexture;

void main()
{
    float depthValue = texture(shadowMapTexture, textureCoords).r;
    outColor = vec4(vec3(depthValue), 1.0); // orthographic
}