#pragma once

#include <world/Object3D.hpp>
#include <vector>
#include <Physics/RigidBody.hpp>

namespace world {
class Chest : world::Object3D {
   private:
    glm::mat4 model;
    std::vector<glm::mat4> jointTransforms = std::vector<glm::mat4>(50);
    physics::RigidBody* rigidBody;

   public:
    Chest(glm::mat4 model);
    virtual ~Chest();

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
};

}  // namespace world
