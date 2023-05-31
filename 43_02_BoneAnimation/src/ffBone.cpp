#include "ffBone.h"

namespace FF
{
	ffBone::ffBone(const std::string _name, uint _id, aiNodeAnim* _boneAnim) :
		m_name(_name),
		m_id(_id),
		m_localTransform(1.0f)
	{
		this->initAnimation(_boneAnim);
	}

	void	ffBone::initAnimation(const aiNodeAnim* _channel)
	{
		for (uint _positionIndex = 0; _positionIndex < _channel->mNumPositionKeys; _positionIndex++)
		{
			aiVector3D	_aiPos = _channel->mPositionKeys[_positionIndex].mValue;
			double		_timeStamp = _channel->mPositionKeys[_positionIndex].mTime;

			ffKeyPosition	_data;
			_data.m_position = ffAssimpHelper::getGLMVec3(_aiPos);
			_data.m_timeStamp = (float)_timeStamp;
			m_positionArr.push_back(_data);
		}

		for (uint _rotationIndex = 0; _rotationIndex < _channel->mNumRotationKeys; _rotationIndex++)
		{
			aiQuaternion	_aiQuat = _channel->mRotationKeys[_rotationIndex].mValue;
			float			_timeStamp = _channel->mRotationKeys[_rotationIndex].mTime;

			ffKeyRotation	_data;
			_data.m_orientation = ffAssimpHelper::getGLMQuat(_aiQuat);
			_data.m_timeStamp = _timeStamp;
			m_rotationArr.push_back(_data);
		}

		for (uint _scaleIndex = 0; _scaleIndex < _channel->mNumScalingKeys; _scaleIndex++)
		{
			aiVector3D		_aiScale = _channel->mScalingKeys[_scaleIndex].mValue;
			double			_timeStamp = _channel->mScalingKeys[_scaleIndex].mTime;

			ffKeyScale		_data;
			_data.m_scale = ffAssimpHelper::getGLMVec3(_aiScale);
			_data.m_timeStamp = _timeStamp;
			m_scaleArr.push_back(_data);
		}
	}

	void ffBone::update(float _time)
	{
		glm::mat4 _translation = this->interpolatePosition(_time);
		glm::mat4 _rotation = this->interpolateRotation(_time);
		glm::mat4 _scale = this->interpolateScale(_time);

		m_localTransform = _translation * _rotation * _scale;
	}

	uint	ffBone::getPositionIndexByTime(float _time)
	{
		for (uint i = 0; i < m_positionArr.size(); i++)
		{
			if (_time < m_positionArr[i + 1].m_timeStamp)
			{
				return i;
			}
		}

		return 0;
	}
	uint	ffBone::getRotationIndexByTime(float _time)
	{
		for (uint i = 0; i < m_rotationArr.size(); i++)
		{
			if (_time < m_rotationArr[i + 1].m_timeStamp)
			{
				return i;
			}
		}
		
		return 0;
	}
	uint	ffBone::getScaleIndexByTime(float _time)
	{
		for (uint i = 0; i < m_scaleArr.size(); i++)
		{
			if (_time < m_scaleArr[i + 1].m_timeStamp)
			{
				return i;
			}
		}
		return 0;
	}
	float	ffBone::getLerpfactor(float _lastTime, float _nextTime, float _curTime)
	{
		assert(_nextTime - _lastTime);
		return (_curTime - _lastTime) / (_nextTime - _lastTime);
	}

	glm::mat4	ffBone::interpolatePosition(float _time)
	{
		if (!m_positionArr.size())
		{
			return glm::mat4(1.0f);
		}

		if (1 == m_positionArr.size())
		{
			return glm::translate(glm::mat4(1.0f), m_positionArr[0].m_position);
		}

		uint _lastIndex = this->getPositionIndexByTime(_time);
		uint _nextIndex = _lastIndex + 1;
		float _factor = getLerpfactor(m_positionArr[_lastIndex].m_timeStamp,
			m_positionArr[_nextIndex].m_timeStamp,
			_time);
		glm::vec3 _result = glm::mix(m_positionArr[_lastIndex].m_position,
			m_positionArr[_nextIndex].m_position,
			_factor);

		return glm::translate(glm::mat4(1.0f), _result);
	}

	glm::mat4	ffBone::interpolateRotation(float _time)
	{
		if (!m_rotationArr.size())
		{
			return glm::mat4(1.0f);
		}

		if (1 == m_rotationArr.size())
		{
			glm::quat _quat = m_rotationArr[0].m_orientation;
			return glm::mat4_cast(_quat);
		}

		uint _lastIndex = this->getRotationIndexByTime(_time);
		uint _nextIndex = _lastIndex + 1;
		float _factor = getLerpfactor(m_rotationArr[_lastIndex].m_timeStamp,
			m_rotationArr[_nextIndex].m_timeStamp,
			_time);
		glm::quat _result = glm::slerp(m_rotationArr[_lastIndex].m_orientation,
			m_rotationArr[_nextIndex].m_orientation,
			_factor);

		return glm::mat4_cast(_result);
	}

	glm::mat4	ffBone::interpolateScale(float _time)
	{
		if (!m_scaleArr.size())
		{
			return glm::mat4(1.0f);
		}

		if (1 == m_scaleArr.size())
		{
			return glm::scale(glm::mat4(1.0f), m_scaleArr[0].m_scale);
		}

		uint _lastIndex = this->getScaleIndexByTime(_time);
		uint _nextIndex = _lastIndex + 1;
		float _factor = getLerpfactor(m_scaleArr[_lastIndex].m_timeStamp,
			m_scaleArr[_nextIndex].m_timeStamp,
			_time);
		glm::vec3 _result = glm::mix(m_scaleArr[_lastIndex].m_scale,
			m_scaleArr[_nextIndex].m_scale,
			_factor);

		return glm::scale(glm::mat4(1.0f), _result);
	}
}