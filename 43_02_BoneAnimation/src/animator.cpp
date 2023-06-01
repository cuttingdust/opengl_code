#include "animator.h"
#include "assimHelper.hpp"

void Animator::playAnimation(Animation *_pAnimation) {
    currentAnimation_ = _pAnimation;
    currentTime_ = 0.0f;
}

void Animator::updateAnimation(float _dt, std::map<std::string, BoneInfo> &_boneInfoMap) {
    deltaTime_  = _dt;
    if (currentAnimation_)
    {
        currentTime_ += currentAnimation_->getTickPerSecond() * _dt;
        currentTime_ = fmod(currentTime_, currentAnimation_->getDuration());
        this->calcuateBoneTransform(&currentAnimation_->getRootNode(), glm::mat4(1.0), _boneInfoMap);
    }
}

std::vector<glm::mat4> Animator::getFinalBoneMatrice() {
    return finalBoneMatrices_;
}

Animator::Animator(Animation *_animation) {
    currentTime_ = 0.0f;
    currentAnimation_ = _animation;

    finalBoneMatrices_.reserve(150);
    for (unsigned  int i = 0; i < 150; i++) {
        finalBoneMatrices_.push_back(glm::mat4(1.0f));
    }
}

void Animator::calcuateBoneTransform(const AssimpNodeData *_pNode, glm::mat4 _parentTransform, std::map<std::string, BoneInfo> &_boneInfoMap) {
    std::string _nodeName = _pNode->name_;
    glm::mat4  _nodeTransform = _pNode->transform_;

    Bone* _bone = currentAnimation_->findBone(_nodeName);
    if (_bone)
    {
        _bone->update(currentTime_);
        _nodeTransform = _bone->getLocalTransform();
    }

    glm::mat4 _globalTransformMatrix = _parentTransform * _nodeTransform;

    if (_boneInfoMap.find(_nodeName) != _boneInfoMap.end())
    {
        int _boneIndex = _boneInfoMap[_nodeName].id_;
        glm::mat4 _offsetMatrx = _boneInfoMap[_nodeName].offsetMatrix_;
        finalBoneMatrices_[_boneIndex] = _globalTransformMatrix * _offsetMatrx;
    }

    for (unsigned int i = 0; i < _pNode->children_.size(); ++i) {
        this->calcuateBoneTransform(&_pNode->children_[i], _globalTransformMatrix, _boneInfoMap);
    }
}

