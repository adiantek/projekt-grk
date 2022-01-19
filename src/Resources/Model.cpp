#include <iostream>
#include <assimp/postprocess.h>

#include <Logger.h>
#include <Resources/Mesh.hpp>
#include <Resources/Model.hpp>
#include <assimp/Importer.hpp>
#include <iostream>
#include <vector>

void Model::loadModel(const char* filename) {
    this->file = std::string(filename);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);

    if (!scene || !scene->mRootNode) {
        LOGE("ERROR::ASSIMP Could not load model: %s\n", importer.GetErrorString());
    } else {
        this->processNode(scene->mRootNode, scene, aiMatrix4x4());
    }
}

void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 transformation) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        transformation *= node->mTransformation;
        Mesh* processedMesh = processMesh(mesh, scene, transformation);
        this->meshes.push_back(processedMesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, transformation);
    }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 transformation) {
    glm::vec3 extents;
    glm::vec3 origin;

    std::vector<Vertex> vertices = this->vertices(mesh, extents, origin, transformation);

    std::string name = mesh->mName.C_Str();
    Mesh* finalMesh = new Mesh(name);

    finalMesh->calculateRenderContext(mesh);

    return finalMesh
        ->setExtents(extents)
        ->setOrigin(origin)
        ->setVertices(vertices);
        // ->setRootJoint(rootJoint);

}

std::vector<Vertex> Model::vertices(aiMesh* mesh, glm::vec3& extents, glm::vec3& origin, aiMatrix4x4 transformation) {
    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector3;
        aiVector3D v = transformation * mesh->mVertices[i];

        // Vertices
        vector3.x = v.x;
        vector3.y = v.y;
        vector3.z = v.z;

        vertex.position = vector3;

        // Normals
        if (mesh->mNormals) {
            vector3.x = mesh->mNormals[i].x;
            vector3.y = mesh->mNormals[i].y;
            vector3.z = mesh->mNormals[i].z;
            vertex.normal = vector3;
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vector2;

            vector2.x = mesh->mTextureCoords[0][i].x;
            vector2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vector2;
        } else {
            vertex.texCoord = glm::vec2(0, 0);
        }

        if (mesh->mTangents) {
            vector3.x = mesh->mTangents[i].x;
            vector3.y = mesh->mTangents[i].y;
            vector3.z = mesh->mTangents[i].z;
            vertex.tangent = vector3;
        }

        // Bitangent
        if (mesh->mBitangents) {
            vector3.x = mesh->mBitangents[i].x;
            vector3.y = mesh->mBitangents[i].y;
            vector3.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector3;
        }

        vertices.push_back(vertex);
    }

    glm::vec3 min = glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z);
    glm::vec3 max = glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z);

    extents = (max - min) * 0.5f;
    origin = glm::vec3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);

    return vertices;
}

std::vector<Mesh*> Model::getMeshes() {
    return this->meshes;
}