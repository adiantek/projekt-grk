#pragma once

#include <world/Object3D.hpp>
#include <Resources/Model.hpp>
#include <Physics/RigidBody.hpp>

namespace world {
class Stone : world::Object3D {
   private:
    glm::mat4 modelMatrix;
    int model;
   public:
    /**
     * @brief Construct a new Stone object
     * 
     * @param modelMatrix 
     * @param model from 0 to 2
     */
    Stone(glm::mat4 modelMatrix, int model, float scale);
    ~Stone();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;

    physics::RigidBody* rigidBody;
};
}  // namespace world
