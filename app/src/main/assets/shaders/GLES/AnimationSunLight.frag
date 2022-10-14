#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;
in vec3 normal;
in vec3 fragPosition;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec3 sunLightDir;
uniform vec3 cameraPosition;

uniform float ambientLight;
uniform float specularLightStrength;

void main()
{
    // diffuse
    float diffuse = max(dot(normal, -sunLightDir), 0.0f);

    // specular
    vec3 fragToCameraDir = normalize(cameraPosition - fragPosition);
    vec3 reflectDir = reflect(sunLightDir, normal); // reflect(pointFrom, normal);
    float specular = pow(max(dot(fragToCameraDir, reflectDir), 0.0f), 32.0f) * specularLightStrength;

    vec3 textureCollor = texture(diffuseTexture, textureCoords).rgb * ambientLight;
    outColor = vec4((ambientLight + diffuse + specular) * textureCollor, 1.0f);
}