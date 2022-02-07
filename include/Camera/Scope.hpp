#pragma once

#include <glm/glm.hpp>

class Scope {
   public:
    Scope(float size = 0.015f, glm::vec3 color = glm::vec3(0.0f));
    ~Scope();
    void draw();

    float size;
    glm::vec3 color;
};

extern Scope* scope;