#include <Logger.h>
#include <Render_Utils.h>
#include <opengl.h>

#include <Animator/Joint.hpp>
#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <functional>
#include <glm/ext.hpp>
#include <iostream>
#include <map>
#include <string>
#include <utils/Cube.hpp>
#include <utils/Gizmos.hpp>
#include <utils/Line.hpp>
#include <vector>

using namespace utils;

void Gizmos::init() {}

void Gizmos::draw() {
    glDisable(GL_DEPTH_TEST);
    for (auto callback : Gizmos::onDrawEvents) {
        callback();
    }
    glEnable(GL_DEPTH_TEST);
}

void Gizmos::onDraw(std::function<void()> callback) {
    Gizmos::onDrawEvents.push_back(callback);
}

void Gizmos::cube(glm::vec3 position) {
    Gizmos::cube(position, glm::vec3(0.05f));
}

void Gizmos::cube(glm::vec3 position, glm::vec3 size) {
    Gizmos::cube(position, size, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Gizmos::cube(glm::vec3 position, float size) {
    Gizmos::cube(position, size, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Gizmos::cube(glm::vec3 position, float size, glm::vec3 color) {
    Gizmos::cube(position, glm::vec3(size), color);
}

void Gizmos::cube(glm::vec3 position, glm::vec3 size, glm::vec3 color) {
    utils::Cube::draw(camera->getTransformationMatrix(), position, size, color, false);
}

void Gizmos::line(glm::vec3 start, glm::vec3 end) {
    Gizmos::line(start, end, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Gizmos::line(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    utils::Line::draw(camera->getTransformationMatrix(), start, end, color);
}

void Gizmos::grid(std::string id, float size) {
}

void Gizmos::grid(std::string id, float size, glm::vec3 color) {
}

void Gizmos::printJointsTree(Animator::Joint* joint) {
    Gizmos::printJointsTree(joint, "");
}

void Gizmos::printJointsTree(Animator::Joint* joint, std::string prefix) {
    LOGD("%s (%d) %s", prefix.c_str(), joint->index, joint->getName().c_str());

    for (Animator::Joint* child : joint->children) {
        Gizmos::printJointsTree(child, prefix + "  ");
    }
}

void Gizmos::dumpJoints(std::vector<Animator::Joint *> joints) {
    LOGD("Dumping joints:");
    for (Animator::Joint *joint : joints) {
        const float *v = glm::value_ptr(joint->getTransform());
        LOGD("%d: %s (parent: %s, matrix: %.2f %.2f %.2f %.2f ...)",
            joint->index, joint->getName().c_str(), joint->hasParent() ? joint->parent->getName().c_str() : 0,
            v[0], v[1], v[2], v[3]
        );
    }
}