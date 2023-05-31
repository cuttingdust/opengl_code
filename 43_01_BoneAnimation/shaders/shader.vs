#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in vec4 inBoneWeights;
layout (location = 6) in ivec4 inBoneIds;

out vec2 outTexCoords;
out vec3 outNormal;

const int MAX_BONES = 150;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 totalPos  = vec4(0.0);

    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (inBoneIds[i] < 0)
        {
            continue;
        }

        if (inBoneIds[i] >= MAX_BONES)
        {
            totalPos = vec4(aPos, 1.0);
            break;
        }

        totalPos += inBoneWeights[i] * finalBoneMatrices[inBoneIds[i]] * vec4(aPos, 1.0);
    }


    outTexCoords = aTexCoords;
    outNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * totalPos;
}
