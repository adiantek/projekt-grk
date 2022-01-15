#include "Render_Utils.h"

#include <Logger.h>
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

    std::vector<float> textureCoord;
    std::vector<unsigned int> indices;
    //tex coord must be converted to 2d vecs
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if (mesh->mTextureCoords[0] != nullptr) {
            textureCoord.push_back(mesh->mTextureCoords[0][i].x);
            textureCoord.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            textureCoord.push_back(0.0f);
            textureCoord.push_back(0.0f);
        }
    }
    if (mesh->mTextureCoords[0] == nullptr) {
        LOGW("no uv coords");
    }
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
    buffer.configureVAO(0, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->pos);
    buffer.configureVAO(1, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->normal);
    buffer.configureVAO(2, 2, GL_FLOAT, GL_FALSE, buffer.getFormat()->tex);
    buffer.configureVAO(3, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->tangent);
    buffer.configureVAO(4, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->bitangent);
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
    buffer.configureVAO(0, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->pos);
    buffer.configureVAO(1, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->normal);
    buffer.configureVAO(2, 2, GL_FLOAT, GL_FALSE, buffer.getFormat()->tex);
    buffer.configureVAO(3, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->tangent);
    buffer.configureVAO(4, 3, GL_FLOAT, GL_FALSE, buffer.getFormat()->bitangent);
}

void Core::DrawVertexArray(const float* vertexArray, int numVertices, int elementSize) {
    glVertexAttribPointer(0, elementSize, GL_FLOAT, false, 0, vertexArray);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Core::DrawVertexArrayIndexed(const float* vertexArray, const int* indexArray, int numIndexes, int elementSize) {
    glVertexAttribPointer(0, elementSize, GL_FLOAT, false, 0, vertexArray);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, indexArray);
}

void Core::DrawVertexArray(const VertexData& data) {
    int numAttribs = std::min(8, data.NumActiveAttribs);
    for (int i = 0; i < numAttribs; i++) {
        glVertexAttribPointer(i, data.Attribs[i].Size, GL_FLOAT, false, 0, data.Attribs[i].Pointer);
        glEnableVertexAttribArray(i);
    }
    glDrawArrays(GL_TRIANGLES, 0, data.NumVertices);
}

void Core::DrawContext(Core::RenderContext& context) {
    glBindVertexArray(context.vertexArray);
    glDrawElements(
        GL_TRIANGLES,     // mode
        context.size,     // count
        GL_UNSIGNED_INT,  // type
        (void*)0          // element array buffer offset
    );
    glBindVertexArray(0);
}
