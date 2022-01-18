#pragma once

#include <opengl.h>

#include <world/Object3D.hpp>

namespace cam {

class Skybox : world::Object3D {
   public:
    Skybox();
    virtual ~Skybox();

    void update() override;
    void draw(glm::mat4 mat) override;

   private:
    GLuint VAO = 0;
    GLuint vbo;
};

}  // namespace cam