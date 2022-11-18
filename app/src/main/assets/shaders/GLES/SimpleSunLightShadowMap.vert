#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextureCoords;

out vec2 textureCoords;
out vec3 normal;
out vec3 fragPos;
out vec4 fragPosLightPerspective;
out float calculateShadows;

uniform mat4 MVPMatrix;
uniform mat4 MVPLightMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main()
{
    textureCoords = inTextureCoords;
    normal = normalize(normalMatrix * inNormal);

    fragPos = (modelMatrix * vec4(inPosition, 1.0f)).xyz;
    fragPosLightPerspective = (MVPLightMatrix * vec4(inPosition, 1.0f));
    calculateShadows = 0.0f; // = false by default
    if(fragPosLightPerspective.x >= -1.0f && fragPosLightPerspective.x <= 1.0f &&
       fragPosLightPerspective.y >= -1.0f && fragPosLightPerspective.y <= 1.0f &&
       fragPosLightPerspective.z >= -1.0f && fragPosLightPerspective.z <= 1.0f)
    {
        // Perspective divide to transform vertex vrom clip space to NDC(-1.0f 1.0f) then to (0.0f 1.0f)
        fragPosLightPerspective.xyz = (fragPosLightPerspective.xyz / fragPosLightPerspective.w) * 0.5 + 0.5;

        calculateShadows = 1.0f; // = true. fragment visible from light point of view. calculate shadows
    }

    gl_Position = MVPMatrix * vec4(inPosition, 1.0f);
}