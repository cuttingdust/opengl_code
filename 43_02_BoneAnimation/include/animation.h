#ifndef __ANIMATION_H__
#define __ANIMATION_H__

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

#include "bone.h"

struct AssimpNodeData {
    glm::mat4                       transform_;
    std::string                     name_;
    std::vector<AssimpNodeData>     children_;
    
    AssimpNodeData(): transform_(1.0), name_(""){children_.clear();}
};

using  BoneInfoMap = std::map<std::string, BoneInfo>;

class Animation
{
public:
    Animation() = default;
    Animation(const aiAnimation* _aiAnim, aiNode* _root, BoneInfoMap& _boneInfoMap, unsigned int _boneCounter);

    inline float getTickPerSecond() {return ticksPerSecond_;}
    inline float getDuration() {return duration_;}
    inline const AssimpNodeData& getRootNode() {return  rootNode_;}

    Bone* findBone(std::string _name);
private:
    void readAnimationData(const aiAnimation* _aiAnim, BoneInfoMap& _boneInfoMap, unsigned int _boneCounter);
    void readHeirachyData(AssimpNodeData& _dst, const aiNode* _src);


private:
    float                               duration_;
    float                               ticksPerSecond_;
    std::vector<Bone>                   boneVec_;
    BoneInfoMap                         boneInfoMap_;
    AssimpNodeData                      rootNode_;
};


#endif
