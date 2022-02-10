#pragma once

#include <world/Object3D.hpp>
#include <Physics/RigidBody.hpp>

namespace world {

class Coin : world::Object3D {
   private:
    glm::mat4 model;
    float openingAnimationStage = 0.0f;

   public:
    Coin(glm::mat4 model);
    ~Coin();

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;

    physics::RigidBody* rigidBody;
};
}  // namespace world