#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "animation.h"

class Animator
{
public:
    Animator() = default;

    explicit Animator(Animation* _animation);
    ~Animator() {}

    void playAnimation(Animation* _pAnimation);
    void updateAnimation(float  _dt, std::map<std::string, BoneInfo>& _boneInfoMap);
    void calcuateBoneTransform(const AssimpNodeData* _pNode, glm::mat4 _parentTransform, std::map<std::string, BoneInfo>& _boneInfoMap);

    std::vector<glm::mat4> getFinalBoneMatrice();

private:
    std::vector<glm::mat4> finalBoneMatrices_;
    Animation*             currentAnimation_;
    float                  currentTime_;
    float                  deltaTime_;
};


#endif
