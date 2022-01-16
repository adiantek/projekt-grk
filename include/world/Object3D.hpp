#pragma once

#include <glm/glm.hpp>

namespace world {

class Object3D {
   public:
    Object3D();
    virtual ~Object3D();

    virtual void update();
    virtual void draw(glm::mat4 mat);
};

}