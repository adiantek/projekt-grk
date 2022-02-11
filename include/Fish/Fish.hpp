#pragma once

#include <Physics/RigidBody.hpp>
#include <Resources/Model.hpp>
#include <world/Object3D.hpp>
#include <world/World.hpp>

namespace fish {

class Fish : world::Object3D {
   public:
    Fish(Model* model, glm::vec3 position, world::World* world, glm::vec3 scale = glm::vec3(1.0f));
    ~Fish();
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    void update() override;
    void setTarget(glm::vec3 target);
    virtual glm::vec3 findTarget() = 0;

    physics::RigidBody* rigidBody;

   protected:
    Model* model;
    world::World* world;
    glm::vec3 target;
    Random* random;
};

}  // namespace fish