#pragma once

#include <glm/ext.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec3 normal;
    glm::vec2 texCoord;
};