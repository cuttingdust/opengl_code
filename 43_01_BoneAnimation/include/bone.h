#ifndef __BONE_H__
#define __BONE_H__

#include "shader.h"
#include "mesh.h"


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

struct BoneInfo {
    unsigned int id_;
    glm::mat4 offsetMatrix_;
    
    BoneInfo(): id_(0), offsetMatrix_(1.0f){}
};


struct KeyPosition{
    glm::vec3 position_;
    float timeStamp_;
};

struct KeyRotation {
    glm::quat orientation_;
    float timeStamp_;
};

struct KeyScale {
    glm::vec3 scale_;
    float timeStamp_;
};


class Bone
{
public:
    explicit Bone(std::string _name, unsigned int _id, aiNodeAnim* _boneAnim);
    ~Bone() = default;
    
public:
    void initAnimation(const aiNodeAnim* _channel);
    void update(float _time);
    
    glm::mat4 getLocalTransform() {
        return  localTransForm_;
    }
    
    unsigned int getId() {
        return id_;
    }
    
    
    std::string getName() {
        return  name_;
    }
    
private:
    float getLerpFactor(float _lastTime, float _nextTime, float _curTime);
    
    glm::mat4 interpolatePosition(float _time);
    glm::mat4 interpolateRotation(float _time);
    glm::mat4 interpolateScale(float _time);
    
    
    unsigned int getPositionIndexByTime(float _time);
    unsigned int getRotationIndexByTime(float _time);
    unsigned int getScaleIndexByTime(float _time);
    
private:
    std::vector<KeyPosition> postionVec_;
    std::vector<KeyRotation> rotationVec_;
    std::vector<KeyScale> scaleVec_;
    
    glm::mat4 localTransForm_;
    std::string name_;
    unsigned int id_;
    
    
    
};





#endif
