#pragma once

#include <world/Object3D.hpp>
#include <world/World.hpp>
#include <Physics/RigidBody.hpp>
#include <Resources/Model.hpp>

class Fish : world::Object3D {
   public:
    Fish(Model* model, glm::vec3 position, world::World* world);
    ~Fish();
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    void update() override;

    physics::RigidBody* rigidBody;

   private:
    Model* model;
    world::World* world;
};