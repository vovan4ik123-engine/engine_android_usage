#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform float loadingProgressInScreenSpace;

void main()
{
    if(gl_FragCoord.x < loadingProgressInScreenSpace)
    {
        outColor = texture(diffuseTexture, textureCoords);
    }
    else
    {
        outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}