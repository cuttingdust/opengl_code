#include"ffModel.h"

namespace FF
{
	void ffModel::loadModel(std::string _path)
	{
		Assimp::Importer _importer;
		const aiScene* _scene = _importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode)
		{
			std::cout << "model read fail!" << std::endl;
			return;
		}

		m_dir = _path.substr(0, _path.find_last_of('/'));
		processNode(_scene->mRootNode, _scene);
		
		aiAnimation* _animation0 = _scene->mAnimations[0];
		m_animation = new ffAnimation(_animation0, _scene->mRootNode, m_boneInfoMap, m_boneCounter);
		m_animator = new ffAnimator(m_animation);
	}

	void ffModel::processNode(aiNode* _node, const aiScene* _scene)
	{
		for (uint i = 0; i < _node->mNumMeshes; i++)
		{
			aiMesh* _mesh = _scene->mMeshes[_node->mMeshes[i]];
			m_meshVec.push_back(processMesh(_mesh, _scene ));
		}

		for (uint i = 0; i < _node->mNumChildren; i++)
		{
			processNode(_node->mChildren[i], _scene);
		}
	}
	ffMesh ffModel::processMesh(aiMesh* _mesh, const aiScene* _scene )
	{
		std::vector<ffVertex>	_vertexVec;
		std::vector<uint>		_indexVec;
		std::vector<ffTexture>	_texVec;

		for (uint i = 0; i < _mesh->mNumVertices; i++)
		{
			ffVertex   _vertex;
			this->setVertexBoneDataToDefault(_vertex);
			
			//位置信息读取
			glm::vec3 _pos;
			_pos.x = _mesh->mVertices[i].x;
			_pos.y = _mesh->mVertices[i].y;
			_pos.z = _mesh->mVertices[i].z;
			_vertex.m_pos = _pos;

			//法线
			glm::vec3 _normal;
			_normal.x = _mesh->mNormals[i].x;
			_normal.y = _mesh->mNormals[i].y;
			_normal.z = _mesh->mNormals[i].z;
			_vertex.m_normal = _normal;

			//纹理坐标
			if (_mesh->mTextureCoords[0])
			{
				glm::vec2 _texCoord;
				_texCoord.x = _mesh->mTextureCoords[0][i].x;
				_texCoord.y = _mesh->mTextureCoords[0][i].y;
				_vertex.m_texCoord = _texCoord;
			}

			_vertexVec.push_back(_vertex);
		}

		//解析index
		for (uint i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace	_face = _mesh->mFaces[i];

			for (uint j = 0; j < _face.mNumIndices; j++)
			{
				_indexVec.push_back(_face.mIndices[j]);
			}
		}

		//解析材质
		if (_mesh->mMaterialIndex >= 0)
		{
			aiMaterial* _mat = _scene->mMaterials[_mesh->mMaterialIndex];

			std::vector<ffTexture> _diffuseVec = loadMaterialTextures(_mat, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE_STR , _scene);
			_texVec.insert(_texVec.end(), _diffuseVec.begin(), _diffuseVec.end());

			std::vector<ffTexture> _specularVec = loadMaterialTextures(_mat, aiTextureType_SPECULAR, TEXTURE_SPECULAR_STR, _scene);
			_texVec.insert(_texVec.end(), _specularVec.begin(), _specularVec.end());
		}

		//解析mesh为基础的骨骼
		loadBoneWeightForVertices(_vertexVec, _mesh, _scene);

		return ffMesh(_vertexVec, _indexVec, _texVec);
	}

	std::vector<ffTexture>	ffModel::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName, const aiScene* _scene)
	{
		std::vector<ffTexture> _texVec;

		for (uint i = 0; i < _mat->GetTextureCount(_type); i++)
		{
			ffTexture _tex;

			aiString _path;
			_mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, i), _path);
			
			if (const aiTexture* _aiTexture = _scene->GetEmbeddedTexture(_path.C_Str()))
			{
				_tex.m_id = ffTextureMananger::getInstance()->createTextureFromMemory(_path.C_Str() , reinterpret_cast<unsigned char*>(_aiTexture->pcData) , _aiTexture->mWidth , _aiTexture->mHeight);
				_tex.m_path = _path.C_Str();
				_tex.m_type = _typeName;
			}
			else
			{
				_tex.m_id = ffTextureMananger::getInstance()->createTexture(_path.C_Str(), m_dir);
				_tex.m_path = _path.C_Str();
				_tex.m_type = _typeName;
			}
			_texVec.push_back(_tex);
		}
		return _texVec;
	}

	void ffModel::loadBoneWeightForVertices(std::vector<ffVertex>& _vertexArr, aiMesh* _pMesh, const aiScene* _pScene)
	{
		for (uint _boneIndex = 0; _boneIndex < _pMesh->mNumBones; _boneIndex++)
		{
			int _boneID = -1;
			std::string _boneName = _pMesh->mBones[_boneIndex]->mName.C_Str();
			if (m_boneInfoMap.find(_boneName) == m_boneInfoMap.end())
			{
				ffBoneInfo _boneInfo;
				_boneInfo.m_id = m_boneCounter;
				_boneInfo.m_offsetMatrix = ffAssimpHelper::getGLMMat4(_pMesh->mBones[_boneIndex]->mOffsetMatrix);
				m_boneInfoMap[_boneName] = _boneInfo;

				_boneID = m_boneCounter;
				m_boneCounter++;
			}
			else
			{
				_boneID = m_boneInfoMap[_boneName].m_id;
			}

			assert(_boneID != -1);
			
			aiVertexWeight* _weights	= _pMesh->mBones[_boneIndex]->mWeights;
			uint			_weightsNum = _pMesh->mBones[_boneIndex]->mNumWeights;
			for (uint _weightIndex = 0; _weightIndex < _weightsNum; _weightIndex++)
			{
				int		_vertexId = _weights[_weightIndex].mVertexId;
				float	_vertexWeight = _weights[_weightIndex].mWeight;
				assert(_vertexId < _vertexArr.size());
				setVertexBoneData(_vertexArr[_vertexId], _boneID, _vertexWeight);
			}
		}
	}

	void ffModel::update(float _deltaTime)
	{
		if (m_animator)
		{
			m_animator->updateAnimation(_deltaTime, m_boneInfoMap);
		}
	}

	void ffModel::draw(Shader& _shader)
	{
		auto _transforms = m_animator->getFinalBoneMatrices();
		for (uint i = 0; i < _transforms.size(); i++)
		{
			_shader.setMatrix("_finalBoneMatrices[" + std::to_string(i) + "]", _transforms[i]);
		}
		for (uint i = 0; i < m_meshVec.size(); i++)
		{
			m_meshVec[i].draw(_shader);
		}
	}

	void ffModel::setVertexBoneDataToDefault(ffVertex _vertex)
	{
		for (uint i = 0; i < MAX_BONE_WEIGHTS; ++i)
		{
			_vertex.m_boneWeightArr[i] = 0.0f;
			_vertex.m_boneIdArr[i] = -1;
		}
	}
	
	void ffModel::setVertexBoneData(ffVertex& _vertex, int _boneID, float _weight)
	{
		for (uint i = 0; i < MAX_BONE_WEIGHTS; ++i)
		{
			if (_vertex.m_boneIdArr[i] < 0)
			{
				_vertex.m_boneWeightArr[i] = _weight;
				_vertex.m_boneIdArr[i] = _boneID;
				break;
			}
		}
	}

	SINGLE_INSTANCE_SET(ffTextureMananger)

	uint ffTextureMananger::createTexture(std::string _path)
	{
		std::map<std::string, uint>::iterator _it = m_texMap.find(_path);
		if (_it != m_texMap.end())
		{
			return _it->second;
		}

		ffImage* _image = ffImage::readFromFile(_path.c_str());

		uint _texID = makeGLTexture(_image);

		delete _image;
		m_texMap[_path] = _texID;
		return _texID;
	}

	uint ffTextureMananger::createTextureFromMemory(std::string _path, unsigned char* _data, uint _width, uint _height)
	{
		std::map<std::string, uint>::iterator _it = m_texMap.find(_path);
		if (_it != m_texMap.end())
		{
			return _it->second;
		}
		ffImage* _image = ffImage::readFromMemory(_data, _width, _height);

		uint _texID = makeGLTexture(_image);

		delete _image;
		m_texMap[_path] = _texID;
		return _texID;
	}

	uint ffTextureMananger::makeGLTexture(ffImage* _pImage)
	{
		if (!_pImage)
		{
			return 0;
		}
		uint _texID = 0;
		glGenTextures(1, &_texID);
		glBindTexture(GL_TEXTURE_2D, _texID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(), _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _pImage->getData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return _texID;
	}

	uint ffTextureMananger::createTexture(std::string _path, std::string _dir)
	{
		return createTexture(_dir + '/' + _path);
	}
}
