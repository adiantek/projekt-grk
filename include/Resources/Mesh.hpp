#pragma once

#include <vector>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Resources/Vertex.hpp>

class Mesh {
   public:
    Mesh(std::string name);
    ~Mesh();

    Mesh* setOrigin(glm::vec3 origin);
    Mesh* setVertices(std::vector<Vertex> vertices);
    Mesh* setExtents(glm::vec3 extents);

    void calculateRenderContext(aiMesh* mesh);
    Core::RenderContext* getRenderContext();

    std::string getName();

    // TODO: Make private
    std::vector<Vertex> vertices;

   private:
    std::string name;
    glm::vec3 origin;
    glm::vec3 extents;
    Core::RenderContext renderContext;

};