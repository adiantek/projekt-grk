#pragma once

#include <stdbool.h>
#include <glm/ext.hpp>

class Robot {
  public:
    // SPEEDS
    inline static const float DEFAULT_MOVEMENT_SPEED = 0.005f;
    inline static const float MAX_SPEED_INCREASED = 0.01f;

    // MODES
    static const int MODE_STATIONARY = 0;
    static const int MODE_WALKING = 1;
    static const int MODE_SWIMMING = 2;
    static const int MODE_SLEEPING = 3;

    Robot();
    ~Robot();

    void update();

    void setMoveDirectionVector(glm::vec3 direction);

    void toggleIncreasedSpeedMode();
    void enableIncreasedSpeedMode();
    void disableIncreasedSpeedMode();
    
    glm::vec3 position;
    glm::vec3 rotation;

  private:
    glm::vec3 moveDirectionVector;

    int mode;
    float movementSpeed;
    float movementVector;

    void draw();
    // GameObject* gameObject;
};

extern Robot *robot;