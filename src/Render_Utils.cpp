#include "Render_Utils.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <vector>
#include <vertex/VertexBuffer.hpp>

using namespace vertex;

void Core::RenderContext::initFromAssimpMesh(aiMesh* mesh) {
    vertexArray = 0;
    vertexBuffer = 0;
    vertexIndexBuffer = 0;

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    VertexBuffer buffer(&POS_NORMAL_TEX_TANGENT_BITANGENT, mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->mTextureCoords[0] != nullptr) {
            buffer.tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        if (mesh->mVertices != nullptr) {
            buffer.pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        }
        if (mesh->mNormals != nullptr) {
            buffer.normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        if (mesh->mTangents != nullptr) {
            buffer.tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }
        if (mesh->mBitangents != nullptr) {
            buffer.bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        buffer.end();
    }

    unsigned int vertexElementBufferSize = sizeof(unsigned int) * (unsigned int)indices.size();
    size = (unsigned int)indices.size();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElementBufferSize, &indices[0], GL_STATIC_DRAW);

    vertexBuffer = buffer.uploadVBO();
    buffer.configurePos(0);
    buffer.configureNormal(1);
    buffer.configureTex(2);
    buffer.configureTangent(3);
    buffer.configureBitangent(4);
}

void Core::RenderContext::initFromAssimpMeshWithArmature(aiMesh* mesh, std::unordered_map<std::string, int> bonesIds) {
    vertexArray = 0;
    vertexBuffer = 0;
    vertexIndexBuffer = 0;

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    int* jointsIds = new int[mesh->mNumVertices * 3];
    float* jointsWeights = new float[mesh->mNumVertices * 3];
    for (unsigned int i = 0; i < mesh->mNumVertices * 3; i++) {
        jointsIds[i] = 0;
        jointsWeights[i] = 0.0f;
    }
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        aiBone *bone = mesh->mBones[i];
        aiVertexWeight *weightsArray = bone->mWeights;
        std::string boneName = bone->mName.C_Str();
        int boneID = bonesIds[boneName];

        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            aiVertexWeight weight = weightsArray[j];
            unsigned int vertexId = weight.mVertexId;
            float weightValue = weight.mWeight;

            int smallest = 0;
            for (int i = 1; i < 3; i++) {
                if (jointsWeights[vertexId * 3 + i] < jointsWeights[vertexId * 3 + smallest]) {
                    smallest = i;
                }
            }

            jointsIds[vertexId * 3 + smallest] = boneID;
            jointsWeights[vertexId * 3 + smallest] = weightValue;
        }
    }

    VertexBuffer buffer(&POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT, mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->mTextureCoords[0] != nullptr) {
            buffer.tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        if (mesh->mVertices != nullptr) {
            buffer.pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        }
        if (mesh->mNormals != nullptr) {
            buffer.normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        if (mesh->mTangents != nullptr) {
            buffer.tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }
        if (mesh->mBitangents != nullptr) {
            buffer.bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        int vertexJointID[3];
        float vertexJointWeight[3];
        for (int j = 0; j < 3; j++) {
            vertexJointID[j] = jointsIds[i * 3 + j];
            vertexJointWeight[j] = jointsWeights[i * 3 + j];
        }

        buffer.joint(vertexJointID, vertexJointWeight);
        buffer.end();
    }

    unsigned int vertexElementBufferSize = sizeof(unsigned int) * (unsigned int)indices.size();
    size = (unsigned int)indices.size();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElementBufferSize, &indices[0], GL_STATIC_DRAW);

    vertexBuffer = buffer.uploadVBO();
    buffer.configurePos(0);
    buffer.configureNormal(1);
    buffer.configureTex(2);
    buffer.configureTangent(3);
    buffer.configureBitangent(4);
    buffer.configureJoint(5, 6);
}

void Core::RenderContext::initPlane(float width, float height, int widthSegments, int heightSegments) {
    vertexArray = 0;
    vertexBuffer = 0;
    vertexIndexBuffer = 0;

    VertexBuffer buffer(&POS_NORMAL_TEX_TANGENT_BITANGENT, (heightSegments + 1) * (widthSegments + 1));

    std::vector<unsigned int> indices;

    float segmentWidth = width / (float)widthSegments;
    float segmentHeight = height / (float)heightSegments;

    float widthHalf = width / 2.0f;
    float heightHalf = height / 2.0f;

    for (int iy = 0; iy < heightSegments + 1; ++iy) {
        float y = (float)iy * segmentHeight - heightHalf;
        for (int ix = 0; ix < widthSegments + 1; ++ix) {
            float x = (float)ix * segmentWidth - widthHalf;
            (&buffer)
                ->pos(x, y, 0.0f)
                ->normal(0.0f, 0.0f, 1.0f)
                ->tex((float)ix / (float)widthSegments, (float)iy / (float)heightSegments)
                ->tangent(1.0f, 0.0f, 0.0f)
                ->bitangent(0.0f, -1.0f, 0.0f)
                ->end();
        }
    }

    for (int iy = 0; iy < heightSegments; ++iy) {
        for (int ix = 0; ix < widthSegments; ++ix) {
            unsigned int a = ix + (widthSegments + 1) * iy;
            unsigned int b = ix + (widthSegments + 1) * (iy + 1);
            unsigned int c = (ix + 1) + (widthSegments + 1) * (iy + 1);
            unsigned int d = (ix + 1) + (widthSegments + 1) * iy;

            // GL_CCW = GL_BACK

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    size = (unsigned int)indices.size();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size, &indices[0], GL_STATIC_DRAW);

    vertexBuffer = buffer.uploadVBO();
    buffer.configurePos(0);
    buffer.configureNormal(1);
    buffer.configureTex(2);
    buffer.configureTangent(3);
    buffer.configureBitangent(4);
}

void Core::DrawContext(Core::RenderContext& context) {
    glBindVertexArray(context.vertexArray);
    glDrawElements(
        GL_TRIANGLES,     // mode
        context.size,     // count
        GL_UNSIGNED_INT,  // type
        (void*)0          // element array buffer offset
    );
}
