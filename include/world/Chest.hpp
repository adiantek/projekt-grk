#pragma once

#include <world/Object3D.hpp>

namespace world {
class Chest : world::Object3D {
   private:
    glm::mat4 model;

   public:
    Chest(glm::mat4 model);
    virtual ~Chest();

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
};

}  // namespace world
