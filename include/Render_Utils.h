#pragma once
#include "glm/glm.hpp"
#include <opengl.h>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Core
{
	struct RenderContext
    {
		GLuint vertexArray;
		GLuint vertexBuffer;
		GLuint vertexIndexBuffer;
		int size = 0;

		void initFromAssimpMesh(aiMesh* mesh);
		void initFromAssimpMeshWithArmature(aiMesh* mesh, std::unordered_map<std::string, int> bonesIds);

		void initPlane(float width, float height, int widthSegments=1, int heightSegments=1);
	};


	struct VertexAttribute
	{
		const void * Pointer;
		int Size;
	};

	struct VertexData
	{
		static const int MAX_ATTRIBS = 8;
		VertexAttribute Attribs[MAX_ATTRIBS];
		int NumActiveAttribs;
		int NumVertices;
	};
	void DrawContext(RenderContext& context);
}