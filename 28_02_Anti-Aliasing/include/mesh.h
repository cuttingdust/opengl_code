#ifndef __MESH_H__
#define __MESH_H__

#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 position_;
    // normal
    glm::vec3 normal_;
    // texCoords
    glm::vec2 textCoords_;
    // tangent
    glm::vec3 tangent_;
    // bitangent
    glm::vec3 bitangent_;
    //bone indexes which will influence this vertex
    int boneIDs_[MAX_BONE_INFLUENCE];
    //weights from each bone
    float weights_[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id_;
    std::string type_;
    std::string path_;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // render the mesh
    void Draw(Shader &shader ) ;

public:
    /*  网格数据  */
    std::vector<Vertex>       vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture>      textures_;
    unsigned int VAO_;

private:
    /*  渲染数据  */
    unsigned int VBO_;
    unsigned int EBO_;

    // initializes all the buffer objects/arrays
    void setupMesh();
};


#endif
