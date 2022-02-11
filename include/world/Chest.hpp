#pragma once

#include <world/Object3D.hpp>
#include <world/Coin.hpp>
#include <vector>
#include <stdbool.h>
#include <Physics/RigidBody.hpp>
#include <Animator/Joint.hpp>
#include <Random.hpp>

namespace world {
class Chest : world::Object3D {
   private:
    int coins;
    glm::mat4 model;
    std::vector<glm::mat4> jointTransforms = std::vector<glm::mat4>(50);
    physics::RigidBody* rigidBody;
    Animator::Joint *coverJoint;
    float openingAnimationStage = 0.0f;
    bool isOpen = false;
    inline static Random *random = new Random(12345);

   public:
    Chest(glm::mat4 model, int coins);
    ~Chest();

    void toggle();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    glm::vec3 getPosition();
};

}  // namespace world
