#pragma once

#include <Fish/Fish.hpp>

class RedSnapper : public Fish {
   public:
    RedSnapper(glm::vec3 position, world::World* world);
    ~RedSnapper();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    glm::vec3 findTarget() override;
    
   private:
    glm::mat4 rotation = glm::eulerAngleX(glm::radians(-90.0f));
};