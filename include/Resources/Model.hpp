#pragma once

#include <opengl.h>
#include <vector>
#include <glm/ext.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <Resources/Mesh.hpp>
#include <Resources/Vertex.hpp>

class Model {
    public:
        void loadModel(char* filename);
        void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation);
        Mesh* processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
        std::vector<Vertex> vertices(aiMesh* mesh, glm::vec3& extents, glm::vec3 &origin, aiMatrix4x4 transformation);
        std::vector<Mesh*> getMeshes();
        std::vector<Mesh*> meshes;
        std::string directory;
        std::string file;
    private:
        
        
};