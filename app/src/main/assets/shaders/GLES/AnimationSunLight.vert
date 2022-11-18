#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextureCoords;
layout(location = 3) in ivec4 inBoneIDs; // INT pointer here
layout(location = 4) in vec4 inWeights;

out vec2 textureCoords;
out vec3 normal;
out vec3 fragPos;

const int MAX_BONES = 31; // 31 bones should be enough for most models

uniform mat4 bonesMatrices[MAX_BONES];
uniform mat4 MVPMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main()
{
    // animation must contain minimum 0 bones per vertex. Maximum = 4
    mat4 boneTransf;
    if(inBoneIDs[0] != -1 && inWeights[0] != -1.0f)
    {
        boneTransf = bonesMatrices[inBoneIDs[0]] * inWeights[0];

        if(inBoneIDs[1] != -1 && inWeights[1] != -1.0f)
        {
            boneTransf += bonesMatrices[inBoneIDs[1]] * inWeights[1];

            if(inBoneIDs[2] != -1 && inWeights[2] != -1.0f)
            {
                boneTransf += bonesMatrices[inBoneIDs[2]] * inWeights[2];

                if(inBoneIDs[3] != -1 && inWeights[3] != -1.0f)
                {
                    boneTransf += bonesMatrices[inBoneIDs[3]] * inWeights[3];
                }
            }
        }
    }
    vec4 posTransformedByBone = boneTransf * vec4(inPosition, 1.0f);

    textureCoords = inTextureCoords;
    normal = normalize(normalMatrix * mat3(boneTransf) * inNormal);

    fragPos = (modelMatrix * posTransformedByBone).xyz;

    gl_Position = MVPMatrix * posTransformedByBone;
}
