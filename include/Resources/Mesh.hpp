#pragma once

#include <map>
#include <vector>
#include <glm/ext.hpp>
#include <stdbool.h>

#include <Render_Utils.h>
#include <Resources/Vertex.hpp>
#include <Animator/Joint.hpp>

class Mesh {
   public:
    Mesh(std::string name);
    ~Mesh();

    Mesh* setOrigin(glm::vec3 origin);
    Mesh* setVertices(std::vector<Vertex> vertices);
    Mesh* setExtents(glm::vec3 extents);
    Mesh* setRootJoint(Animator::Joint* rootJoint);
    Mesh* setJointsCount(int jointsCount);
    Mesh* setJoints(std::vector<Animator::Joint*> joints);

    void calculateRenderContext(aiMesh* mesh, std::map<std::string, int> bonesIds);

    Core::RenderContext* getRenderContext();
    std::vector<Vertex> getVertices();
    Animator::Joint* getRootJoint();
    std::vector<Animator::Joint*> getJoints();
    std::string getName();
    int getJointsCount();

    bool hasJoints();
   private:
    std::string name;

    /* -- Not implemented yet */
    glm::vec3 origin;
    /* -- Not implemented yet */
    glm::vec3 extents;
    std::vector<Vertex> vertices;

    Core::RenderContext renderContext;
    Animator::Joint* rootJoint;
    std::vector<Animator::Joint*> joints;

    int jointsCount = 0;
};