#pragma once

#include <opengl.h>

#include <world/Object3D.hpp>

namespace world {  // powinno byc to na etapie GUI rysowane, ale nie mamy kamery na kwaternionach

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