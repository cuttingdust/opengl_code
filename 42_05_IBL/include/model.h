#ifndef __MODEL_H__
#define __MODEL_H__

#include "shader.h"
#include "mesh.h"


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
    
    /// constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool gamma = false) ;
    
    /// draws the model, and thus all its meshes
    void Draw(Shader &shader);
    
    
private:
    /// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);
    
    /// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);
    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
   
    /// checks all material textures of a given type and loads the textures if they're not loaded yet.
    /// the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    
    
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
    
    
public:
    bool bGammaCorrection_;
    std::string directory_;
    
    /// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes_;
    
    /// model data
    std::vector<Texture> texturesLoaded_;

};





#endif
