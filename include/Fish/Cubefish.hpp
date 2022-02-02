#pragma once

#include <world/Object3D.hpp>
#include <Physics/RigidBody.hpp>
#include <glm/glm.hpp>

class Cubefish : world::Object3D {
   public:
    Cubefish(glm::vec3 position, float mass, float density);
    ~Cubefish();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;

   private:
    physics::RigidBody* rigidBody;
};