#pragma once

#include <Fish/Fish.hpp>

class Pilotfish : public Fish {
   public:
    Pilotfish(glm::vec3 position, world::World* world);
    ~Pilotfish();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
};