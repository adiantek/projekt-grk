#include <iostream>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <string>
#include <vector>
#include <map>
#include <glm/ext.hpp>
#include <functional>

#include <Logger.h>
#include <Resources/Mesh.hpp>
#include <Resources/Model.hpp>
#include <Animator/Joint.hpp>
#include <Gizmos/Gizmos.hpp>

void Model::loadModel(char* filename) {
    printf("Processing model: %s\n", filename);
    this->file = std::string(filename);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace |/*aiProcess_FlipUVs |  | aiProcess_GenBoundingBoxes |*/ aiProcess_PopulateArmatureData);

    printf("MODEL AFTER READFILE\n");

    if (!scene || !scene->mRootNode) {
        LOGE("ERROR::ASSIMP Could not load model: %s\n", importer.GetErrorString());
    } else {
        this->processNode(scene->mRootNode, scene, aiMatrix4x4());
        this->joints = this->loadJoints(scene);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        transformation *= node->mTransformation;
        Mesh* processedMesh = this->loadMesh(mesh, scene, transformation);
        this->meshes.push_back(processedMesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, transformation);
    }
}

Mesh* Model::loadMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation) {
    printf("Processing mesh: %s\n", mesh->mName.C_Str());
    glm::vec3 extents;
    glm::vec3 origin;

    // std::vector<Vertex> vertices = this->vertices(mesh, extents, origin, transformation);

    std::string name = mesh->mName.C_Str();
    Mesh* finalMesh = new Mesh(name);

    finalMesh->calculateRenderContext(mesh);

    printf("[] Model class work success!...\n");
    return finalMesh;
        // ->setAssimpMesh(mesh);
        // ->setExtents(extents)
        // ->setOrigin(origin)
        // ->setVertices(vertices);
}

// Map vertices to a vector of Vertex (currently not used)
// std::vector<Vertex> Model::vertices(aiMesh* mesh, glm::vec3& extents, glm::vec3& origin, aiMatrix4x4 transformation) {
//     std::vector<Vertex> vertices;

//     for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//         Vertex vertex;
//         glm::vec3 vector3;
//         aiVector3D v = transformation * mesh->mVertices[i];

//         // Vertices
//         vector3.x = v.x;
//         vector3.y = v.y;
//         vector3.z = v.z;

//         vertex.position = vector3;

//         // Normals
//         if (mesh->mNormals) {
//             vector3.x = mesh->mNormals[i].x;
//             vector3.y = mesh->mNormals[i].y;
//             vector3.z = mesh->mNormals[i].z;
//             vertex.normal = vector3;
//         }

//         // Texture coordinates
//         if (mesh->mTextureCoords[0]) {
//             glm::vec2 vector2;

//             vector2.x = mesh->mTextureCoords[0][i].x;
//             vector2.y = mesh->mTextureCoords[0][i].y;
//             vertex.texCoord = vector2;
//         } else {
//             vertex.texCoord = glm::vec2(0, 0);
//         }

//         if (mesh->mTangents) {
//             vector3.x = mesh->mTangents[i].x;
//             vector3.y = mesh->mTangents[i].y;
//             vector3.z = mesh->mTangents[i].z;
//             vertex.tangent = vector3;
//         }

//         // Bitangent
//         if (mesh->mBitangents) {
//             vector3.x = mesh->mBitangents[i].x;
//             vector3.y = mesh->mBitangents[i].y;
//             vector3.z = mesh->mBitangents[i].z;
//             vertex.bitangent = vector3;
//         }

//         vertices.push_back(vertex);
//     }

//     glm::vec3 min = glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z);
//     glm::vec3 max = glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z);

//     extents = (max - min) * 0.5f;
//     origin = glm::vec3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);

//     return vertices;
// }

std::vector<Mesh*> Model::getMeshes() {
    return this->meshes;
}

std::vector<Animator::Joint*> Model::loadJoints(const aiScene* scene) {
    std::map<std::string, aiBone*> bones = {};

    // Iterate over all meshes and add bones to 
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        if (mesh->HasBones()) {
            for (unsigned int j = 0; j < mesh->mNumBones; j++) {
                aiBone* bone = mesh->mBones[j];
                std::string name = bone->mName.C_Str();

                std::map<std::string, aiBone*>::iterator it;

                it = bones.find(name);

                // Bone already exists
                if (it != bones.end()) continue;

                bones.insert(std::pair<std::string, aiBone*>(name, bone));
            }
        }
    }

    // Create final joints vector
    std::vector<Animator::Joint*> _joints = {};
    std::map<std::string, int> ids = {};

    int index = 0;

    for (auto const& [key, bone] : bones) {
        std::cout << key << ':' << bone << std::endl;
        glm::mat4 transform = Model::to_mat4(bone->mNode->mTransformation);
        Animator::Joint* newJoint = new Animator::Joint(index, key, transform);
        newJoint->setLocalBindTransform(Model::to_mat4(bone->mOffsetMatrix));
        ids.insert(std::pair<std::string, int>(key, index));
        _joints.push_back(newJoint);
        index++;
    }

    // Add root joint to front
    Animator::Joint* rootJoint = new Animator::Joint(index, "root", glm::mat4(1.0f));

    // Populate parents and children
    for (auto const& [key, bone] : bones) {
        std::cout << key << ':' << bone << std::endl;
        
        for (unsigned int i = 0; i < bone->mNode->mNumChildren; i++) {
            aiNode* child = bone->mNode->mChildren[i];
            std::string childName = child->mName.C_Str();
            std::map<std::string, int>::iterator it;

            it = ids.find(childName);

            if (it != ids.end()) {
                _joints[ids[key]]->addChild(_joints[ids[childName]]);
            }
        }
        
        if (bone->mNode->mParent != nullptr) {
            std::string parentName = bone->mNode->mParent->mName.C_Str();
            std::map<std::string, int>::iterator it;

            it = ids.find(parentName);

            if (it != ids.end()) {
                _joints[ids[key]]->setParent(_joints[ids[parentName]]);
            } else {
                _joints[ids[key]]->setParent(rootJoint);
                rootJoint->addChild(_joints[ids[key]]);
            }
        }
        
    }

    _joints.insert(_joints.begin(), rootJoint);

    // Print all transforms
    for (Animator::Joint* joint : _joints) {
        std::cout << joint->name << ": " << glm::to_string(joint->getLocalBindTransform()).c_str() << std::endl;
    }

    rootJoint->calculateInverseBindTransform(glm::mat4());

    if (bones.size() > 0) {
        Gizmos::printJointsTree(rootJoint);
    }

    return _joints;
}

std::vector<Animator::Joint*> Model::getJoints() {
    return this->joints;
}

/** Convert from a row-major ASSIMP matrix to a column-major GLM matrix */
glm::mat4 Model::to_mat4(const aiMatrix4x4& aAssimpMat) {
    return glm::transpose(glm::make_mat4(&aAssimpMat.a1));
}