#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;
in vec3 fragPosTangentSpace;
in vec3 sunLightDirTangentSpace;
in vec3 cameraPosTangentSpace;
in vec4 fragPosLightPerspective;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform lowp sampler2DShadow shadowMapTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D heightMapTexture;

uniform float ambientLight;
uniform float specularLightStrength;

uniform float heightScale;

void main()
{
    vec3 fragToCameraDir = normalize(cameraPosTangentSpace - fragPosTangentSpace);

    // height mapping
    float height = texture(heightMapTexture, textureCoords).r;
    vec2 calculatedTextCoords = textureCoords - fragToCameraDir.xy * (height * heightScale);

    // normal mapping
    vec3 normal = texture(normalMapTexture, calculatedTextCoords).rgb * 2.0f - 1.0f;   // this normal is in tangent space
    normal = normalize(normal);

    // diffuse
    float diffuse = max(dot(normal, -sunLightDirTangentSpace), 0.0f);

    // specular
    vec3 reflectDir = reflect(sunLightDirTangentSpace, normal); // reflect(fromLightPosToFragPos, normal);
    float specular = pow(max(dot(fragToCameraDir, reflectDir), 0.0f), 32.0f) * specularLightStrength;

    // shadow
    float shadowMultiplier = textureProj(shadowMapTexture, fragPosLightPerspective);

    vec3 textureCollor = texture(diffuseTexture, calculatedTextCoords).rgb * 0.8f;
    outColor = vec4((ambientLight + (diffuse + specular) * shadowMultiplier) * textureCollor, 1.0f);
}