#pragma once
#include"ffAnimation.h"

namespace FF
{
	class ffAnimator
	{
	public:
		ffAnimator(ffAnimation * _animation);
		~ffAnimator() {}

		void playAnimation(ffAnimation* _pAnim);
		void updateAnimation(float dt, std::map<std::string, ffBoneInfo>& _boneInfoMap);
		void calculateBoneTransform(const ffAssimpNodeData* _pNode , glm::mat4 _parentTransform, std::map<std::string, ffBoneInfo>& _boneInfoMap);
		std::vector<glm::mat4> getFinalBoneMatrices();
	private:
		std::vector<glm::mat4> m_finalBoneMatArr;
		ffAnimation* m_currentAnim;
		float m_currentTime;
		float m_deltaTime;
	};
}


