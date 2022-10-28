#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextureCoords;
layout(location = 3) in vec3 inTangent;

out vec2 textureCoords;
out vec3 fragPosTangentSpace;
out vec3 sunLightDirTangentSpace;
out vec3 cameraPosTangentSpace;
out vec4 fragPosLightPerspective;

uniform mat4 MVPMatrix;
uniform mat4 MVPLightMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform vec3 sunLightDir;
uniform vec3 cameraPos;

void main()
{
    textureCoords = inTextureCoords;
    vec3 fragPos = (modelMatrix * vec4(inPosition, 1.0f)).xyz;
    fragPosLightPerspective = (MVPLightMatrix * vec4(inPosition, 1.0f)) * 0.5 + 0.5;

    // lighting
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N); // re-orthogonalize T with respect to N
    vec3 B = cross(N, T); // dont need normalize. sin 90 degrees = 1 = vec length
    mat3 TBN = transpose(mat3(T, B, N)); // matrix to move vector in tangent psace

    // move vectors for light calculation to tangent space
    fragPosTangentSpace = TBN * fragPos;
    sunLightDirTangentSpace = normalize(TBN * sunLightDir);
    cameraPosTangentSpace = TBN * cameraPos;

    gl_Position = MVPMatrix * vec4(inPosition, 1.0f);
}