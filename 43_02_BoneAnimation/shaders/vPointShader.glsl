#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec4  inBoneWeights;
layout (location = 4) in ivec4 inBoneIds;


out vec2 outUV;
out vec3 outFragPos;
out vec3 outNormal;

const int MAX_BONES = 150;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 _finalBoneMatrices[MAX_BONES];


uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main()
{
    vec4 _totalPos = vec4(0.0);
    for(int i = 0;i < MAX_BONE_INFLUENCE;i++)
    {   
        if(inBoneIds[i] < 0)
        {
            continue;
        }

        if(inBoneIds[i] >= MAX_BONES)
        {
            _totalPos = vec4(aPos , 1.0);
            break;
        }
        _totalPos += inBoneWeights[i] * _finalBoneMatrices[inBoneIds[i]] * vec4(aPos , 1.0);
    }
   gl_Position = _projMatrix * _viewMatrix * _modelMatrix * _totalPos;
   outUV = aUV;
   outFragPos = vec3( _modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0));
   outNormal = mat3(transpose(inverse(_modelMatrix))) * aNormal;
}