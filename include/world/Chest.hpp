#pragma once

#include <world/Object3D.hpp>
#include <world/Coin.hpp>
#include <vector>
#include <stdbool.h>
#include <Physics/RigidBody.hpp>
#include <Animator/Joint.hpp>

namespace world {
class Chest : world::Object3D {
   private:
    glm::mat4 model;
    std::vector<glm::mat4> jointTransforms = std::vector<glm::mat4>(50);
    physics::RigidBody* rigidBody;
    Animator::Joint *coverJoint;
    std::vector<world::Coin*> coins;
    float openingAnimationStage = 0.0f;
    bool isOpen = false;

   public:
    Chest(glm::mat4 model);
    ~Chest();

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
};

}  // namespace world
