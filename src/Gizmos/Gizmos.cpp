#include <Render_Utils.h>
#include <opengl.h>
#include <iostream>

#include <Animator/Joint.hpp>
#include <Camera/Camera.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Gizmos/Line.hpp>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <glm/ext.hpp>

void Gizmos::init() {}

void Gizmos::cube(glm::vec3 position) {
    glm::vec3 size = glm::vec3(0.05f);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(position, size, color);
}

void Gizmos::cube(glm::vec3 position, glm::vec3 size) {
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(position, size, color);
}

void Gizmos::cube(glm::vec3 position, float size) {
    glm::vec3 sizeVector = glm::vec3(size);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(position, sizeVector, color);
}

void Gizmos::cube(glm::vec3 position, float size, glm::vec3 color) {
    glm::vec3 sizeVector = glm::vec3(size);

    return Gizmos::cube(position, sizeVector, color);
}

void Gizmos::cube(glm::vec3 position, glm::vec3 size, glm::vec3 color) {
    Gizmos::line(position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), color);
    Gizmos::line(position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), color);
    Gizmos::line(position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), color);
    Gizmos::line(position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), color);

    Gizmos::line(position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color);

    Gizmos::line(position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color);
}

void Gizmos::line(glm::vec3 start, glm::vec3 end) {
    return Gizmos::line(start, end, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Gizmos::line(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    Line* line = new Line(start, end);
    line->setMVP(camera->getTransformationMatrix());
    line->setColor(color);
    line->draw();
}

void Gizmos::joint(glm::vec3 offset, Animator::Joint* joint) {
    glm::mat4 transformation = glm::inverse(joint->getLocalBindTransform());
    Animator::Joint* parent = joint;

    glm::vec3 position = glm::vec3(transformation[3]);

    // for (Animator::Joint* child : joint->children) {
    //     glm::vec3 childPosition = glm::vec3(child->getLocalBindTransform()[3]);
    //     Gizmos::line(position, childPosition);
    // }

    Line* line = new Line(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.1f));
    line->setMVP(camera->getTransformationMatrix() * transformation);
    line->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    line->draw();
}

void Gizmos::grid(float size) {
}

void Gizmos::grid(float size, glm::vec3 color) {
}

void Gizmos::printJointsTree(Animator::Joint* joint) {
    Gizmos::printJointsTree(joint, "");
}

void Gizmos::printJointsTree(Animator::Joint* joint, std::string prefix) {
    std::cout << prefix << joint->name << std::endl;

    for (Animator::Joint* child : joint->children) {
        Gizmos::printJointsTree(child, prefix + "  ");
    }
}