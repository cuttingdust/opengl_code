#pragma once
#include"Base.h"
#include"Shader.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffAssimpHelper.h"
#include"ffImage.h"
#include"ffMesh.h"
#include"ffBone.h"
#include"ffAnimation.h"
#include"ffAnimator.h"

#define MAX_BONE_WEIGHTS 4
namespace FF
{
	class ffModel
	{
	public:
		ffModel(const char * _path)
		{
			loadModel(_path);
		}
		void draw(Shader& _shader);
		void update(float _deltaTime);
	private:
		std::vector<ffMesh>			m_meshVec;
		std::string					m_dir;
		ffAnimation*				m_animation;
		ffAnimator*					m_animator;

		/*--------------------¹Ç÷À¶¯»­-----------------*/
		std::map<std::string, ffBoneInfo>	m_boneInfoMap;
		uint								m_boneCounter = 0;

		void loadBoneWeightForVertices(std::vector<ffVertex>& _vertexArr, aiMesh* _pMesh, const aiScene* _pScene);
		
		/*--------------------¹Ç÷À¶¯»­-----------------*/


		void setVertexBoneDataToDefault(ffVertex _vertex);
		void setVertexBoneData(ffVertex& _vertex , int _boneID , float _weight);

		void loadModel(std::string _path);
		void processNode(aiNode * _node , const aiScene * _scene);
		ffMesh processMesh(aiMesh* _mesh, const aiScene* _scene);
		std::vector<ffTexture>	loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName, const aiScene* _scene);
		
	};

	class ffTextureMananger
	{
	public:
		void SINGLE_OVER() {}
		uint createTexture(std::string _path);
		uint createTexture(std::string _path , std::string _dir);
		uint createTextureFromMemory(std::string _path, unsigned char* _data, uint _width, uint _height);
	private:
		SINGLE_INSTANCE(ffTextureMananger)
		ffTextureMananger() {}
		uint makeGLTexture(ffImage* _pImage);

		std::map<std::string, uint>	m_texMap;
	};
}