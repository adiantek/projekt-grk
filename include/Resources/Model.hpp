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
#include <Animator/Joint.hpp>

class Model {
    public:
        void loadModel(char* filename);
        void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation);

        // TODO: Change to loadVertices()
        // std::vector<Vertex> vertices(aiMesh* mesh, glm::vec3& extents, glm::vec3 &origin, aiMatrix4x4 transformation);

        std::vector<Mesh*> getMeshes();
        std::vector<Animator::Joint*> getJoints();
        
        std::string file;

        // TODO: Move this to other place (helpers functions)
        static glm::mat4 to_mat4(const aiMatrix4x4& aAssimpMat);
    private:
        Mesh* loadMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
        std::vector<Animator::Joint*> loadJoints(const aiScene* scene);

        std::vector<Mesh*> meshes;
        std::vector<Animator::Joint*> joints;
};