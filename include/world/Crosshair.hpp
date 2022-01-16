#pragma once

#include <opengl.h>

#include <world/Object3D.hpp>

namespace world {

class Crosshair : Object3D {
   private:
    GLuint vao;
    GLuint vbo;

   public:
    Crosshair();
    virtual ~Crosshair();

    void update() override;
    void draw(glm::mat4 mat) override;
};

}  // namespace world