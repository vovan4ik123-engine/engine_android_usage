#version 300 es

precision highp float; // highp mediump lowp

in vec2 textureCoords;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightPerspective;
in float calculateShadows;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMapTexture;

uniform vec3 sunLightDir;
uniform vec3 cameraPos;

uniform float ambientLight;
uniform float specularLightStrength;

void main()
{
    // diffuse
    float diffuse = max(dot(normal, -sunLightDir), 0.0f);

    // specular
    vec3 fragToCameraDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(sunLightDir, normal); // reflect(fromLightPosToFragPos, normal);
    float specular = pow(max(dot(fragToCameraDir, reflectDir), 0.0f), 32.0f) * specularLightStrength;

    // shadow
    float shadowMultiplier = 1.0f;
    if(calculateShadows == 1.0f) // calculateShadows == true
    {
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture(shadowMapTexture, fragPosLightPerspective.xy).r;
        // get depth of current fragment from light's perspective
        float currentDepth = fragPosLightPerspective.z;
        shadowMultiplier = closestDepth < currentDepth ? 0.0f : 1.0f;
    }

    vec3 textureCollor = texture(diffuseTexture, textureCoords).rgb * 0.8f;
    outColor = vec4((ambientLight + (diffuse + specular) * shadowMultiplier) * textureCollor, 1.0f);
}