#pragma once

#include <opengl.h>
#include <vector>
#include <map>
#include <string>
#include <stdbool.h>
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
        bool hasJoints();

        std::vector<Mesh*> getMeshes();
        std::vector<Animator::Joint*> getJoints();
        Animator::Joint* getRootJoint();
        Animator::Joint* getJoint(std::string name);
        
        std::string file;

        // TODO: Move this to other place (helpers functions)
        static glm::mat4 to_mat4(const aiMatrix4x4& aAssimpMat);
    private:
        void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation);
        Mesh* loadMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
        std::vector<Animator::Joint*> loadJoints(const aiScene* scene);

        std::vector<Mesh*> meshes;
        std::vector<Animator::Joint*> joints;
        std::map<std::string, int> bonesIds;
};