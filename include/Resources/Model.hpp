#pragma once

#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <opengl.h>
#include <stdbool.h>

#include <Animator/Joint.hpp>
#include <Resources/Mesh.hpp>
#include <Resources/Vertex.hpp>
#include <assimp/Importer.hpp>
#include <glm/ext.hpp>
#include <map>
#include <string>
#include <vector>
#include <PxPhysicsAPI.h>

class Model {
   public:
    void loadModel(const char* filename);
    bool hasJoints();

    std::vector<Mesh*> getMeshes();
    std::vector<Animator::Joint*> getJoints();
    Animator::Joint* getRootJoint();
    Animator::Joint* getJoint(std::string name);
    physx::PxTriangleMeshGeometry createGeometry(glm::vec3 scale = glm::vec3(1.0f));
    physx::PxBoxGeometry createGeometryAABB(glm::vec3 scale = glm::vec3(1.0f));

    std::string file;

    // TODO: Move this to other place (helpers functions)
    static glm::mat4 to_mat4(const aiMatrix4x4& aAssimpMat);

   private:
    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation);
    Mesh* loadMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation);
    std::vector<Animator::Joint*> loadJoints(const aiScene* scene);

    std::vector<Mesh*> meshes;
    std::vector<Animator::Joint*> joints;
    std::unordered_map<std::string, int> bonesIds;

    physx::PxVec3 aabbMin = physx::PxVec3(2000000000.0f);
    physx::PxVec3 aabbMax = physx::PxVec3(-2000000000.0f);
};