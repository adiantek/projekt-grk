#pragma once

#include <stdbool.h>

#include <Resources/GameObject.hpp>
#include <glm/ext.hpp>
#include <world/Object3D.hpp>

namespace entity {

class Robot : world::Object3D {
   public:
    // SPEEDS
    inline static const float DEFAULT_SWIMMING_MOVEMENT_SPEED = 1.0f;
    inline static const float MAX_SWIMMING_SPEED_INCREASED = 2.0f * 10;

    inline static const float DEFAULT_WALKING_MOVEMENT_SPEED = 1.0f;
    inline static const float MAX_WALKING_SPEED_INCREASED = 1.5f * 10;

    inline static const float ROTATION_SPEED = 20.0f;

    // MODES
    static const int MODE_STATIONARY = 0;
    static const int MODE_WALKING = 1;
    static const int MODE_SWIMMING = 2;
    static const int MODE_SLEEPING = 3;

    float currentSpeed = 0;
    float currentTurnSpeed = 0;

    Robot();
    virtual ~Robot();

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;

    void setMoveDirectionVector(glm::vec3 direction);

    void toggleIncreasedSpeedMode();
    void enableIncreasedSpeedMode();
    void disableIncreasedSpeedMode();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 direction;

   private:
    glm::vec3 moveDirectionVector;

    int mode;
    float movementSpeed;
    float movementVector;

    GameObject* gameObject;
};

}  // namespace entity

extern entity::Robot* robot;