#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextureCoords;

out vec2 textureCoords;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 MVP_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

void main()
{
    textureCoords = inTextureCoords;
    fragPosition = (model_matrix * vec4(inPosition, 1.0f)).xyz;
    normal = normalize(normal_matrix * inNormal);

    gl_Position = MVP_matrix * vec4(inPosition, 1.0f);
}