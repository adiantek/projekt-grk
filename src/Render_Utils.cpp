#include "Render_Utils.h"

#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <Logger.h>

void Core::RenderContext::initFromAssimpMesh(aiMesh* mesh) {
    vertexArray = 0;
    vertexBuffer = 0;
    vertexIndexBuffer = 0;

    std::vector<float> textureCoord;
    std::vector<unsigned int> indices;
    //tex coord must be converted to 2d vecs
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->mTextureCoords[0] != nullptr) {
            textureCoord.push_back(mesh->mTextureCoords[0][i].x);
            textureCoord.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            textureCoord.push_back(0.0f);
            textureCoord.push_back(0.0f);
        }
    }
    if (mesh->mTextureCoords[0] == nullptr) {
        LOGW("no uv coords");
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // TODO refactor this
    // lots of redundant calls to glBufferSubData

    unsigned int vertexDataBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexNormalBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexTexBufferSize = sizeof(float) * mesh->mNumVertices * 2;
    unsigned int vertexTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexBiTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;

    unsigned int vertexElementBufferSize = sizeof(unsigned int) * (unsigned int) indices.size();
    size = (unsigned int) indices.size();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);


    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElementBufferSize, &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    //std::cout << vertexBuffer;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBufferData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize + vertexBiTangentBufferSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataBufferSize, mesh->mVertices);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize, vertexNormalBufferSize, mesh->mNormals);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize, vertexTexBufferSize, &textureCoord[0]);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize, vertexTangentBufferSize, mesh->mTangents);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize, vertexBiTangentBufferSize, mesh->mBitangents);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertexNormalBufferSize + vertexDataBufferSize));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize));

}

void Core::RenderContext::initPlane(float width, float height, int widthSegments, int heightSegments) {
    vertexArray = 0;
    vertexBuffer = 0;
    vertexIndexBuffer = 0;

    std::vector<float> buffer;
    std::vector<unsigned int> indices;

    float segmentWidth = width / (float) widthSegments;
    float segmentHeight = height / (float) heightSegments;

    float widthHalf = width / 2.0f;
    float heightHalf = height / 2.0f;

    for(int iy = 0; iy < heightSegments + 1; ++iy) {
        float y = (float) iy * segmentHeight - heightHalf;
        for(int ix = 0; ix < widthSegments + 1; ++ix) {
            float x = (float) ix * segmentWidth - widthHalf;
            // Vertex
            buffer.push_back(x);
            buffer.push_back(y);
            buffer.push_back(0.0f);
            // Normal
            buffer.push_back(0.0f);
            buffer.push_back(0.0f);
            buffer.push_back(1.0f);
            // UV coords
            buffer.push_back((float) ix / (float) widthSegments);
            buffer.push_back((float) iy / (float) heightSegments);
            // Tangent
            buffer.push_back(1.0f);
            buffer.push_back(0.0f);
            buffer.push_back(0.0f);
            // Bitangent
            buffer.push_back(0.0f);
            buffer.push_back(-1.0f);
            buffer.push_back(0.0f);
        }
    }

    for(int iy = 0; iy < heightSegments; ++iy) {
        for(int ix = 0; ix < widthSegments; ++ix) {
            unsigned int a = ix + (widthSegments + 1) *  iy;
            unsigned int b = ix + (widthSegments + 1) *  (iy + 1);
            unsigned int c = (ix + 1) + (widthSegments + 1) *  (iy + 1);
            unsigned int d = (ix + 1) + (widthSegments + 1) *  iy;

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    size = (unsigned int) indices.size();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size, &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), &buffer[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * 4, (void*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * 4, (void*)(3 * 4));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * 4, (void*)(6 * 4));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * 4, (void*)(8 * 4));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * 4, (void*)(11 * 4));
}

void Core::DrawVertexArray(const float * vertexArray, int numVertices, int elementSize )
{
	glVertexAttribPointer(0, elementSize, GL_FLOAT, false, 0, vertexArray);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Core::DrawVertexArrayIndexed( const float * vertexArray, const int * indexArray, int numIndexes, int elementSize )
{
	glVertexAttribPointer(0, elementSize, GL_FLOAT, false, 0, vertexArray);
	glEnableVertexAttribArray(0);

	glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, indexArray);
}


void Core::DrawVertexArray( const VertexData & data )
{
	int numAttribs = std::min(8, data.NumActiveAttribs);
	for(int i = 0; i < numAttribs; i++)
	{
		glVertexAttribPointer(i, data.Attribs[i].Size, GL_FLOAT, false, 0, data.Attribs[i].Pointer);
		glEnableVertexAttribArray(i);
	}
	glDrawArrays(GL_TRIANGLES, 0, data.NumVertices);
}

void Core::DrawContext(Core::RenderContext& context)
{

	glBindVertexArray(context.vertexArray);
	glDrawElements(
		GL_TRIANGLES,      // mode
		context.size,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);
	glBindVertexArray(0);
}
