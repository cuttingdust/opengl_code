#include "ffAnimator.h"

namespace FF
{
	ffAnimator::ffAnimator(ffAnimation* _animation)
	{
		m_currentTime = 0.0f;
		m_currentAnim = _animation;

		m_finalBoneMatArr.reserve(150);

		for (uint i = 0; i < 150; i++)
		{
			m_finalBoneMatArr.push_back(glm::mat4(1.0f));
		}
	}
	void ffAnimator::playAnimation(ffAnimation* _pAnim)
	{
		m_currentAnim = _pAnim;
		m_currentTime = 0.0f;
	}
	void ffAnimator::updateAnimation(float dt, std::map<std::string, ffBoneInfo>& _boneInfoMap)
	{
		m_deltaTime = dt;
		if (m_currentAnim)
		{
			m_currentTime += m_currentAnim->getTicksPerSecond() * dt;
			m_currentTime = fmod(m_currentTime, m_currentAnim->getDuration());

			calculateBoneTransform(&m_currentAnim->getRootNode(), glm::mat4(1.0f), _boneInfoMap);
		}
	}
	void ffAnimator::calculateBoneTransform(const ffAssimpNodeData* _pNode, glm::mat4 _parentTransform, std::map<std::string, ffBoneInfo>& _boneInfoMap)
	{
		std::string _nodeName = _pNode->m_name;
		glm::mat4	_nodeTransform = _pNode->m_transform;
		if (_nodeName == "mixamorig:RightHandPinky4") {
			std::cout << "";
		}

		ffBone* _bone = m_currentAnim->findBone(_nodeName);
		bool hasAnimation = false;
		if (_bone)
		{
			hasAnimation = true;
			_bone->update(m_currentTime);
			_nodeTransform = _bone->getLocationTransform();
		}

		glm::mat4 _globalTransformMatrix = _parentTransform * _nodeTransform;

		if (_boneInfoMap.find(_nodeName) != _boneInfoMap.end())
		{
			int _boneIndex = _boneInfoMap[_nodeName].m_id;
			glm::mat4 _offsetMatrix = _boneInfoMap[_nodeName].m_offsetMatrix;
			m_finalBoneMatArr[_boneIndex] = _globalTransformMatrix * _offsetMatrix;
		}

		for (uint i = 0; i < _pNode->m_children.size(); i++)
		{
			this->calculateBoneTransform(&_pNode->m_children[i], _globalTransformMatrix, _boneInfoMap);
		}
	}

	std::vector<glm::mat4> ffAnimator::getFinalBoneMatrices()
	{
		return m_finalBoneMatArr;
	}
}