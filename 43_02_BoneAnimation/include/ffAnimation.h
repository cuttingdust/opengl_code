#pragma once

#include"Base.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffAssimpHelper.h"
#include"ffBone.h"

namespace FF
{
	struct ffAssimpNodeData
	{
		glm::mat4						m_transform;
		std::string						m_name;
		std::vector<ffAssimpNodeData>	m_children;
		ffAssimpNodeData() :m_transform(1.0f), m_name("") { m_children.clear(); }
	};

	class ffAnimation
	{
	public:
		ffAnimation() = default;
		ffAnimation(const aiAnimation* _aiAnim , aiNode* _root , std::map<std::string, ffBoneInfo>& _boneInfoMap, uint& _boneCount);
		
		inline float getTicksPerSecond() { return m_ticksPerSecond; }
		inline float getDuration() { return m_duration; }
		inline const ffAssimpNodeData& getRootNode() { return m_rootNode; }

		ffBone* findBone(std::string _name);
	private:
		void readAnimationData(const aiAnimation* _aiAnim , std::map<std::string, ffBoneInfo>& _boneInfoMap, uint& _boneCount);
		void readHeirarchyData(ffAssimpNodeData& _dst , const aiNode* _src);

		float								m_duration;
		float								m_ticksPerSecond;
		std::vector<ffBone>					m_boneArr;
		std::map<std::string, ffBoneInfo>	m_boneInfoMap;
		ffAssimpNodeData					m_rootNode;
	};
}


