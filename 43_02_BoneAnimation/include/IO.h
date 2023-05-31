//#pragma once
//#include"Base.h"
//#include"Shader.h"
//#include"assimp/Importer.hpp"
//#include"assimp/scene.h"
//#include"assimp/postprocess.h"
//#include"ffImage.h"
//namespace FF
//{
//	struct ffVertex
//	{
//		glm::vec3 m_position;
//		glm::vec3 m_normal;
//		glm::vec2 m_texCoord;
//		ffVertex()
//		{
//			m_position	= glm::vec3(1.0f);
//			m_normal	= glm::vec3(1.0f);
//			m_texCoord	= glm::vec3(1.0f);
//		}
//		~ffVertex()
//		{
//		}
//	};
//
//	struct ffTexture
//	{
//		uint			m_id;
//		std::string		m_type;
//		std::string		m_path;
//		ffTexture()
//		{
//			m_id = -1;
//			m_type = "";
//			m_path = "";
//		}
//		~ffTexture()
//		{
//
//		}
//	};
//
//
//#define TEXT_DIFFUSE_STR   "texture_diffuse"
//#define TEXT_SPECULAR_STR  "texture_specular"
//	class ffMesh
//	{
//	public:
//		std::vector<ffVertex>		m_vertexVec;
//		std::vector<uint>			m_indexVec;
//		std::vector<ffTexture>		m_texVec;
//
//		ffMesh(std::vector<ffVertex> _vertexVec, std::vector<uint> _indexVec, std::vector<ffTexture> _texVec);
//		void draw(Shader& _shader);
//	private:
//		uint m_VBO, m_VAO, m_EBO;
//		void setupMesh();
//	};
//
//
//	class ffModel
//	{
//	public:
//		ffModel(const char* _path)
//		{
//			loadModel(_path);
//		}
//		void draw(Shader &_shader);
//
//	private:
//		std::vector<ffMesh> m_meshVec;
//		std::string			m_directory;
//
//		void		loadModel(std::string _path);
//		void		processNode(aiNode* _node, const aiScene* _scene);
//		ffMesh		processMesh(aiMesh* _mesh, const aiScene* _scene);
//		std::vector<ffTexture>  loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, std::string _typeName);
//	};
//	class ffTextureManager
//	{
//		SINGLE_INSTANCE(ffTextureManager)
//	public:
//		void SINGLE_OVER() {}
//		uint	createTexture(std::string _path, std::string _dir);
//		uint	createTexture(std::string _path);
//	private:
//		ffTextureManager() {}
//		std::map<std::string , uint>	m_texMap;
//	};
//}