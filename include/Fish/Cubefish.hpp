#pragma once

#include <world/Object3D.hpp>
#include <Resources/Model.hpp>
#include <Physics/RigidBody.hpp>
#include <glm/glm.hpp>

class Cubefish : world::Object3D {
   public:
    Cubefish(glm::vec3 position, float mass, float density);
    Cubefish(glm::mat4 modelMatrix, float mass, float density, Model* model, bool dynamic);
    ~Cubefish();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    Model* model;

   private:
    physics::RigidBody* rigidBody;
};