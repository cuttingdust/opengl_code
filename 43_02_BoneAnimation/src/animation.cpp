
#include "animation.h"
#include "assimHelper.hpp"

void Animation::readAnimationData(
        const aiAnimation *_aiAnim
        , BoneInfoMap &_boneInfoMap
        , unsigned int _boneCounter) {
    for (unsigned int i = 0; i < _aiAnim->mNumChannels; i++) {
        std::string _boneName = _aiAnim->mChannels[i]->mNodeName.C_Str();
        if (_boneInfoMap.find(_boneName) == _boneInfoMap.end())
        {
            BoneInfo _boneInfo;
            _boneInfo.id_ = _boneCounter;
            _boneCounter++;
            _boneInfoMap[_boneName] = _boneInfo;
        }

        /// 骨骼信息存储
        boneVec_.push_back(Bone(_boneName, _boneInfoMap[_boneName].id_, _aiAnim->mChannels[i]));
    }

    boneInfoMap_ = _boneInfoMap;
}

void Animation::readHeirachyData(AssimpNodeData &_dst, const aiNode *_src) {

    assert(_src);
    _dst.name_ = _src->mName.C_Str();
    _dst.transform_ = AssimpHelper::getGLMMat4(_src->mTransformation);

    for (unsigned int i = 0; i < _src->mNumChildren; i++) {
        AssimpNodeData _newNode;
        this->readHeirachyData(_newNode, _src->mChildren[i]);
        _dst.children_.push_back(_newNode);
    }

}

Bone *Animation::findBone(std::string _name) {
    for (unsigned int i = 0; i < boneVec_.size(); i++) {
        if (boneVec_[i].getName() == _name)
        {
            return &boneVec_[i];
        }
    }


    return nullptr;
}

Animation::Animation(const aiAnimation* _aiAnim, aiNode *_root, BoneInfoMap &_boneInfoMap, unsigned int _boneCounter) {
    duration_ = static_cast<float>(_aiAnim->mDuration);
    ticksPerSecond_ = static_cast<float>(_aiAnim->mTicksPerSecond);
    readHeirachyData(rootNode_, _root);
    readAnimationData(_aiAnim, _boneInfoMap, _boneCounter);
}

