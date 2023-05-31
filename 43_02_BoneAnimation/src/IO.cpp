//#include"IO.h"
//
//namespace FF
//{
//	ffMesh::ffMesh(std::vector<ffVertex> _vertexVec, std::vector<uint> _indexVec, std::vector<ffTexture> _texVec)
//	{
//		this->m_vertexVec = _vertexVec;
//		this->m_indexVec = _indexVec;
//		this->m_texVec = _texVec;
//
//		this->setupMesh();
//	}
//
//	void ffMesh::setupMesh()
//	{
//		glGenVertexArrays(1, &m_VAO);
//		glGenBuffers(1, &m_VBO);
//		glGenBuffers(1, &m_EBO);
//
//		glBindVertexArray(m_VAO);
//
//		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);		
//		glBufferData(GL_ARRAY_BUFFER, sizeof(ffVertex) * m_vertexVec.size(), &m_vertexVec[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_indexVec.size(), &m_indexVec[0], GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)0);
//
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)offsetof(ffVertex, m_normal));	
//
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)offsetof(ffVertex, m_texCoord));
//	
//		glBindVertexArray(0);
//	}
//
//	void ffMesh::draw(Shader& _shader)
//	{
//		uint _diffuseN = 1;
//		uint _specularN = 1;
//
//		for (uint i = 0; i < m_texVec.size(); ++i)
//		{
//			glActiveTexture(GL_TEXTURE0 + i);
//
//			std::string _number;
//			std::string _name = m_texVec[i].m_type;
//			if (_name == TEXT_DIFFUSE_STR)
//			{
//				_number = std::to_string(_diffuseN++);
//			}
//			if (_name == TEXT_SPECULAR_STR)
//			{
//				_number = std::to_string(_specularN++);
//			}
//			_shader.setFloat("myMaterial." + _name + _number, i);
//			glBindTexture(GL_TEXTURE_2D, m_texVec[i].m_id);
//		}
//		glActiveTexture(GL_TEXTURE0);
//
//		glBindVertexArray(m_VAO);
//		glDrawElements(GL_TRIANGLES, m_indexVec.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//	}
//
//
//
//
//	void ffModel::draw(Shader& _shader)
//	{
//		for (uint i = 0; i < m_meshVec.size(); i++)
//		{
//			m_meshVec[i].draw(_shader);
//		}
//	}
//	void ffModel::loadModel(std::string _path)
//	{
//		Assimp::Importer _importer;
//		const aiScene* _scene = _importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//		if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode)
//		{
//			std::cout << "Error ::Assimp" << _importer.GetErrorString() << std::endl;
//			return;
//		}
//
//		m_directory = _path.substr(0, _path.find_last_of('/'));
//
//		processNode(_scene->mRootNode, _scene);	
//	}
//	void ffModel::processNode(aiNode* _node, const aiScene* _scene)
//	{
//		for (uint i = 0; i < _node->mNumMeshes; i++)
//		{
//			aiMesh*  _mesh = _scene->mMeshes[_node->mMeshes[i]];
//			m_meshVec.push_back(processMesh(_mesh, _scene));
//		}
//
//		for (uint i = 0; i < _node->mNumChildren; i++)
//		{
//			processNode(_node->mChildren[i], _scene);
//		}
//	}
//	ffMesh	ffModel::processMesh(aiMesh* _mesh, const aiScene* _scene)
//	{
//		std::vector<ffVertex>	_vertexVec;
//		std::vector<uint>		_indexVec;
//		std::vector<ffTexture>	_texVec;
//
//		for (uint i = 0; i < _mesh->mNumVertices; i++)
//		{
//			ffVertex	_vertex;
//			
//			//位置
//			glm::vec3	_pos;
//			_pos.x = _mesh->mVertices[i].x;
//			_pos.y = _mesh->mVertices[i].y;
//			_pos.z = _mesh->mVertices[i].z;
//			_vertex.m_position = _pos;
//
//			//法线
//			glm::vec3	_normal;
//			_normal.x = _mesh->mNormals[i].x;
//			_normal.y = _mesh->mNormals[i].y;
//			_normal.z = _mesh->mNormals[i].z;
//			_vertex.m_normal = _normal;
//
//			//纹理坐标
//			if (_mesh->mTextureCoords[0])
//			{
//				glm::vec2 _texCoord;
//				_texCoord.x = _mesh->mTextureCoords[0][i].x;
//				_texCoord.y = _mesh->mTextureCoords[0][i].y;
//				_vertex.m_texCoord = _texCoord;
//			}
//			else
//			{
//				_vertex.m_texCoord = glm::vec2(0.0f);
//			}
//
//			_vertexVec.push_back(_vertex);
//		}
//
//		//index
//		for (uint i = 0; i < _mesh->mNumFaces; i++)
//		{
//			aiFace _face = _mesh->mFaces[i];
//			for (uint j = 0; j < _face.mNumIndices; j++)
//			{
//				_indexVec.push_back(_face.mIndices[j]);
//			}
//		}
//
//		//材质
//		if (_mesh->mMaterialIndex >= 0)
//		{
//			aiMaterial* _mat = _scene->mMaterials[_mesh->mMaterialIndex];
//			std::vector<ffTexture>  _diffMaps = loadMaterialTextures(_mat, aiTextureType_DIFFUSE, TEXT_DIFFUSE_STR);
//			_texVec.insert(_texVec.end(), _diffMaps.begin(), _diffMaps.end());
//
//			std::vector<ffTexture>  _specMaps = loadMaterialTextures(_mat, aiTextureType_SPECULAR, TEXT_SPECULAR_STR);
//			_texVec.insert(_texVec.end(), _specMaps.begin(), _specMaps.end());
//		}
//
//		return ffMesh(_vertexVec, _indexVec, _texVec);
//	}
//
//	std::vector<ffTexture>  ffModel::loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, std::string _typeName)
//	{
//		std::vector<ffTexture> _texVec;
//		for (uint i = 0; i < _mat->GetTextureCount(_type); i++)
//		{
//			ffTexture _tex;
//
//			aiString _path;
//			_mat->GetTexture(_type, i, &_path);
//
//			_tex.m_id = ffTextureManager::getInstance()->createTexture(_path.C_Str(), m_directory);
//			_tex.m_path = _path.C_Str();
//			_tex.m_type = _typeName;
//
//			_texVec.push_back(_tex);
//		}
//
//		return _texVec;
//	}
//
//
//	SINGLE_INSTANCE_SET(ffTextureManager)
//
//	uint	ffTextureManager::createTexture(std::string _path, std::string _dir)
//	{
//		return createTexture(_dir + "/" + _path);
//	}
//	uint	ffTextureManager::createTexture(std::string _path)
//	{
//		std::map<std::string, uint>::iterator _it = m_texMap.find(_path);
//		if (_it != m_texMap.end())
//		{
//			return _it->second;
//		}
//
//		ffImage* _image = ffImage::readFromFile(_path.c_str());
//
//		uint _texID = 0;
//		glGenTextures(1, &_texID);
//		glBindTexture(GL_TEXTURE_2D, _texID);
//
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _image->getWidth(), _image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _image->getData());
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		delete _image;
//		m_texMap[_path] = _texID;
//		return _texID;
//	}
//}