#include <string>
#include <vector>

#include <Render_Utils.h>
#include <Resources/Mesh.hpp>
#include <Resources/Vertex.hpp>

Mesh::Mesh(std::string name) {
    this->name = name;
}

Mesh::~Mesh() { }

std::string Mesh::getName() {
    return this->name;
}

Mesh* Mesh::setVertices(std::vector<Vertex> vertices) {
    this->vertices = vertices;
    return this;
}

Mesh* Mesh::setOrigin(glm::vec3 origin) {
    this->origin = origin;
    return this;
}

Mesh* Mesh::setExtents(glm::vec3 extents) {
    this->extents = extents;
    return this;
}

Mesh* Mesh::setRootJoint(Animator::Joint* rootJoint) {
    this->rootJoint = rootJoint;
    return this;
}

void Mesh::calculateRenderContext(aiMesh* mesh) {
    this->renderContext = Core::RenderContext();
    this->renderContext.initFromAssimpMesh(mesh);
}

Core::RenderContext* Mesh::getRenderContext() {
    return &this->renderContext;
}