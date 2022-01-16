#include <Render_Utils.h>
#include <opengl.h>
#include <iostream>
#include <string>
#include <map>

#include <Animator/Joint.hpp>
#include <Camera/Camera.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Gizmos/Line.hpp>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <glm/ext.hpp>

void Gizmos::init() {}

void Gizmos::cube(std::string id, glm::vec3 position) {
    glm::vec3 size = glm::vec3(0.05f);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(id, position, size, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, glm::vec3 size) {
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(id, position, size, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, float size) {
    glm::vec3 sizeVector = glm::vec3(size);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(id, position, sizeVector, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, float size, glm::vec3 color) {
    glm::vec3 sizeVector = glm::vec3(size);

    return Gizmos::cube(id, position, sizeVector, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, glm::vec3 size, glm::vec3 color) {
    Gizmos::line(id + "_1", position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_2", position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_3", position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_4", position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), color);

    Gizmos::line(id + "_5", position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_6", position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_7", position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_8", position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color);

    Gizmos::line(id + "_9", position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_10", position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_11", position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_12", position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color);
}

void Gizmos::line(std::string id, glm::vec3 start, glm::vec3 end) {
    return Gizmos::line(id, start, end, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Gizmos::line(std::string id, glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    std::map<std::string, Line*>::iterator iter = Gizmos::lines.find(id);
    Line* line;

    if (iter != Gizmos::lines.end()) {
        line = iter->second;
        line->setStart(start);
        line->setEnd(end);
        line->setColor(color);
    } else {
        line = new Line(start, end);
        line->setColor(color);

        Gizmos::lines[id] = line;
    }

    line->setMVP(camera->getTransformationMatrix());
    line->draw();
}

void Gizmos::grid(std::string id, float size) {
}

void Gizmos::grid(std::string id, float size, glm::vec3 color) {
}

void Gizmos::printJointsTree(Animator::Joint* joint) {
    Gizmos::printJointsTree(joint, "");
}

void Gizmos::printJointsTree(Animator::Joint* joint, std::string prefix) {
    std::cout << prefix << "(" << joint->index << ") " << joint->name << std::endl;

    for (Animator::Joint* child : joint->children) {
        Gizmos::printJointsTree(child, prefix + "  ");
    }
}