
#pragma once

#include <Render_Utils.h>
#include <stdbool.h>

#include <Animator/Joint.hpp>
#include <Resources/Vertex.hpp>
#include <glm/ext.hpp>
#include <map>
#include <vector>

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

    void calculateRenderContext(aiMesh* mesh, std::unordered_map<std::string, int> bonesIds);

    Core::RenderContext* getRenderContext();
    Animator::Joint* getRootJoint();
    std::vector<Animator::Joint*> getJoints();
    std::string getName();
    int getJointsCount();

    bool hasJoints();

    std::vector<float> vertices;
    std::vector<int> indices;

   private:
    std::string name;

    /* -- Not implemented yet */
    glm::vec3 origin;
    /* -- Not implemented yet */
    glm::vec3 extents;
    std::vector<Vertex> vertices2;

    Core::RenderContext renderContext;
    Animator::Joint* rootJoint;
    std::vector<Animator::Joint*> joints;

    int jointsCount = 0;
};