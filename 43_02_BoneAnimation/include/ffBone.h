#pragma once
#include"Base.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffAssimpHelper.h"

namespace FF
{
	struct ffKeyPosition
	{
		glm::vec3	m_position;
		float		m_timeStamp;
	};

	struct ffKeyRotation
	{
		glm::quat	m_orientation;
		float		m_timeStamp;
	};

	struct ffKeyScale
	{
		glm::vec3	m_scale;
		float		m_timeStamp;
	};
	struct ffBoneInfo
	{
		uint		m_id;
		glm::mat4	m_offsetMatrix;
		ffBoneInfo() :m_id(0), m_offsetMatrix(1.0f) {}
	};

	class ffBone
	{
	private:
		std::vector<ffKeyPosition>	m_positionArr;
		std::vector<ffKeyRotation>	m_rotationArr;
		std::vector<ffKeyScale>		m_scaleArr;

		glm::mat4	m_localTransform;//相对于父节点，自己的本地变换
		std::string	m_name;
		uint		m_id;

	public:
		ffBone(const std::string _name, uint _id , aiNodeAnim* _boneAnim);
		~ffBone() {}
		
		void		initAnimation(const aiNodeAnim* _channel);
		glm::mat4	getLocationTransform() { return m_localTransform; }
		uint		getId() { return m_id; }
		std::string getName() { return m_name; }

		void	update(float _time);

	private:
		float getLerpfactor(float _lastTime, float _nextTime, float _curTime);

		glm::mat4	interpolatePosition(float _time);
		glm::mat4	interpolateRotation(float _time);
		glm::mat4	interpolateScale(float _time);

		uint	getPositionIndexByTime(float _time);
		uint	getRotationIndexByTime(float _time);
		uint	getScaleIndexByTime(float _time);
	};
}


