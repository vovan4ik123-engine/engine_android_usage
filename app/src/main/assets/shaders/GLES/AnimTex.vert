#version 300 es

precision highp float; // highp mediump lowp

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextureCoords;
layout(location = 3) in ivec4 boneIDs; // INT pointer here
layout(location = 4) in vec4 weights;

out vec2 textureCoords;

const int MAX_BONES = 50; // 50 bones should be enough for most models

uniform mat4 bonesMatrices[MAX_BONES];
uniform mat4 MVP_matrix;

void main()
{
    textureCoords = inTextureCoords;

    // animation must contain minimum 0 bones per vertex. Maximum = 4
    mat4 boneTransf;
    if(boneIDs[0] != -1 && weights[0] != -1.0f)
    {
        boneTransf = bonesMatrices[boneIDs[0]] * weights[0];

        if(boneIDs[1] != -1 && weights[1] != -1.0f)
        {
            boneTransf += bonesMatrices[boneIDs[1]] * weights[1];

            if(boneIDs[2] != -1 && weights[2] != -1.0f)
            {
                boneTransf += bonesMatrices[boneIDs[2]] * weights[2];

                if(boneIDs[3] != -1 && weights[3] != -1.0f)
                {
                    boneTransf += bonesMatrices[boneIDs[3]] * weights[3];
                }
            }
        }
    }

    vec4 transformedByBone = boneTransf * vec4(inPosition, 1.0f);

    gl_Position = MVP_matrix * transformedByBone;
}