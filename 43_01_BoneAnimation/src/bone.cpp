#include "bone.h"
#include "assimHelper.hpp"


Bone::Bone(std::string _name, unsigned int _id, aiNodeAnim* _boneAnim)
: name_(_name)
, id_(_id)
, localTransForm_(1.0)
{
    if (_boneAnim) {
        this->initAnimation(_boneAnim);
    }
    
   
}

void Bone::initAnimation(const aiNodeAnim *_channel) {
    
    for (unsigned int _positionIndex = 0; _positionIndex < _channel->mNumPositionKeys; _positionIndex++) {
        aiVector3D _aiPos = _channel->mPositionKeys[_positionIndex].mValue;
        double         _timeStamp = _channel->mPositionKeys[_positionIndex].mTime;
        
        KeyPosition _data;
        _data.position_ = AssimpHelper::getGLMVec3(_aiPos);
        _data.timeStamp_ = static_cast<float>(_timeStamp) ;
        
        postionVec_.push_back(_data);
    }
    
    for (unsigned int _rotationIndex = 0; _rotationIndex < _channel->mNumRotationKeys; _rotationIndex++) {
        aiQuaternion  _aiQuat = _channel->mRotationKeys[_rotationIndex].mValue;
        double              _timeStamp = _channel->mPositionKeys[_rotationIndex].mTime;
        
        KeyRotation _data;
        _data.orientation_ = AssimpHelper::getGLMQuat(_aiQuat);
        _data.timeStamp_ = _timeStamp;
        
        rotationVec_.push_back(_data);
    }
    
    for (unsigned int _scaleIndex = 0; _scaleIndex < _channel->mNumScalingKeys; _scaleIndex++) {
        aiVector3D   _aiScale = _channel->mScalingKeys[_scaleIndex].mValue;
        double           _timeStamp = _channel->mPositionKeys[_scaleIndex].mTime;
        
        KeyScale _data;
        _data.scale_ = AssimpHelper::getGLMVec3(_aiScale);
        _data.timeStamp_ = _timeStamp;
        
        scaleVec_.push_back(_data);
    }
    
    
}

void Bone::update(float _time) { 
    glm::mat4 _tranlation = this->interpolatePosition(_time);
    glm::mat4 _rotation = this->interpolateRotation(_time);
    glm::mat4 _scale = this->interpolateScale(_time);
    
    localTransForm_ = _tranlation * _rotation * _scale;
    
}

float Bone::getLerpFactor(float _lastTime, float _nextTime, float _curTime) { 
    // Replaced assert with an exception for error handling.
        if (_nextTime == _lastTime) {
            throw std::runtime_error("Division by zero in getLerpFactor().");
        }
        
        return  (_curTime - _lastTime) / (_nextTime - _lastTime);
}

glm::mat4 Bone::interpolateScale(float _time) { 
    if (!scaleVec_.size()) {
        return  glm::mat4(1.0);
    }
    
    if (1 == scaleVec_.size()) {
        return  glm::scale(glm::mat4(1.0f), scaleVec_[0].scale_);
    }
    
    /// 找到当前时间的上一帧 与 下一帧
    unsigned int _lastIndex = this->getScaleIndexByTime(_time);
    unsigned int _nextIndex = _lastIndex + 1;
    
    ///计算插值系数
    float _factor = this->getLerpFactor(
                                        scaleVec_[_lastIndex].timeStamp_,
                                        scaleVec_[_nextIndex].timeStamp_,
                                        _time);
    
    ///做插值
    glm::vec3 _result = glm::mix(
                                 scaleVec_[_lastIndex].scale_,
                                 scaleVec_[_nextIndex].scale_,
                                 _factor);
    
    return  glm::scale(glm::mat4(1.0f), _result);
}


glm::mat4 Bone::interpolateRotation(float _time) { 
    if (!rotationVec_.size()) {
        return  glm::mat4(1.0);
    }
    
    if (1 == rotationVec_.size()) {
        glm::quat _quat = rotationVec_[0].orientation_;
        return  glm::mat4_cast(_quat);
    }
    
    /// 找到当前时间的上一帧 与 下一帧
    unsigned int _lastIndex = this->getRotationIndexByTime(_time);
    unsigned int _nextIndex = _lastIndex + 1;
    
    ///计算插值系数
    float _factor = this->getLerpFactor(
                                        rotationVec_[_lastIndex].timeStamp_,
                                        rotationVec_[_nextIndex].timeStamp_,
                                        _time);
    
    ///做插值
    glm::quat _result = glm::slerp(
                                 rotationVec_[_lastIndex].orientation_,
                                 rotationVec_[_nextIndex].orientation_,
                                 _factor);
    
    return  glm::mat4_cast(_result);
}


glm::mat4 Bone::interpolatePosition(float _time) { 
    if (!postionVec_.size()) {
        return  glm::mat4(1.0);
    }
    
    if (1 == postionVec_.size()) {
        return  glm::translate(glm::mat4(1.0f), postionVec_[0].position_);
    }
    
    /// 找到当前时间的上一帧 与 下一帧
    unsigned int _lastIndex = this->getPositionIndexByTime(_time);
    unsigned int _nextIndex = _lastIndex + 1;
    
    ///计算插值系数
    float _factor = this->getLerpFactor(
                                        postionVec_[_lastIndex].timeStamp_,
                                        postionVec_[_nextIndex].timeStamp_,
                                        _time);
    
    ///做插值
    glm::vec3 _result = glm::mix(
                                 postionVec_[_lastIndex].position_,
                                 postionVec_[_nextIndex].position_,
                                 _factor);
    
    return  glm::translate(glm::mat4(1.0f), _result);
}

unsigned int Bone::getPositionIndexByTime(float _time) { 
    for (unsigned int i = 0; i < postionVec_.size() - 1; i++) {  // Subtract 1 here to prevent array out of bounds.
           if (_time < postionVec_[i + 1].timeStamp_) {
               return  i;
           }
       }
       
       return  0;
}

unsigned int Bone::getScaleIndexByTime(float _time) { 
    for (unsigned int i = 0; i < scaleVec_.size() - 1; i++) {  // Subtract 1 here to prevent array out of bounds.
            if (_time < scaleVec_[i + 1].timeStamp_) {
                return  i;
            }
        }
        
        return  0;
}


unsigned int Bone::getRotationIndexByTime(float _time) { 
    for (unsigned int i = 0; i < rotationVec_.size() - 1; i++) {  // Subtract 1 here to prevent array out of bounds.
           if (_time < rotationVec_[i + 1].timeStamp_) {
               return  i;
           }
       }
       
       return  0;
}







