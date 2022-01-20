#include <Render_Utils.h>
#include <stdbool.h>

#include <Animator/Joint.hpp>
#include <Resources/Mesh.hpp>
#include <Resources/Vertex.hpp>
#include <string>
#include <vector>

Mesh::Mesh(std::string name) {
    this->name = name;
}

Mesh::~Mesh() {}

/* -- Basic mesh functions -- */

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

Core::RenderContext* Mesh::getRenderContext() {
    return &this->renderContext;
}

/* -- Joints -- */

void Mesh::calculateRenderContext(aiMesh* mesh, std::unordered_map<std::string, int> bonesIds) {
    this->renderContext = Core::RenderContext();
    this->renderContext.initFromAssimpMeshWithArmature(mesh, bonesIds);
}

Mesh* Mesh::setRootJoint(Animator::Joint* rootJoint) {
    this->rootJoint = rootJoint;
    return this;
}

Mesh* Mesh::setJointsCount(int jointsCount) {
    this->jointsCount = jointsCount;
    return this;
}

Mesh* Mesh::setJoints(std::vector<Animator::Joint*> joints) {
    this->joints = joints;
    return this;
}

Animator::Joint* Mesh::getRootJoint() {
    return this->rootJoint;
}

std::vector<Animator::Joint*> Mesh::getJoints() {
    return this->joints;
}

int Mesh::getJointsCount() {
    return this->jointsCount;
}

bool Mesh::hasJoints() {
    return this->jointsCount > 0;
}