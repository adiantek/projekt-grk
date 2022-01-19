#pragma once

#include <stdbool.h>
#include <glm/ext.hpp>

#include <Resources/GameObject.hpp>

struct RobotLeg {
    int id;

    Animator::Joint *upperJoint;
    Animator::Joint *lowerJoint;

    float upperJointLength = 0.0f;
    float lowerJointLength = 0.0f;

    /* This is the actual point attached to the leg (when move, bones are also moving) */
    glm::vec3 attachmentPoint = glm::vec3(0.0f);
    glm::vec3 attachmentEstimation = glm::vec3(0.0f);

    glm::vec3 currentAttachmentPoint = glm::vec3(-1.0f);
    glm::vec3 targetAttachmentPoint = glm::vec3(0.0f);
    glm::vec3 previousAttachmentPoint = glm::vec3(0.0f);
    float step = -1.0f;

    glm::vec3 lowerJointLengthVector = glm::vec3(0.0f);

    glm::vec3 upperJointOrigin = glm::vec3(0.0f);
    glm::vec3 lowerJointOrigin = glm::vec3(0.0f);
};

class Robot {
  public:
    // SPEEDS
    inline static const float DEFAULT_SWIMMING_MOVEMENT_SPEED = 1.0f;
    inline static const float MAX_SWIMMING_SPEED_INCREASED = 2.0f;

    inline static const float DEFAULT_WALKING_MOVEMENT_SPEED = 1.0f;
    inline static const float MAX_WALKING_SPEED_INCREASED = 1.5f;

    inline static const float ROTATION_SPEED = 20.0f;
    inline static const float LEG_STEP_SPEED = 0.2f;
    inline static const float LEG_MAX_DISTANCE_SQUARE = 0.4f;

    // MODES
    static const int MODE_STATIONARY = 0;
    static const int MODE_WALKING = 1;
    static const int MODE_SWIMMING = 2;
    static const int MODE_SLEEPING = 3;

    float currentSpeed = 0;
    float currentTurnSpeed = 0;

    Robot();
    ~Robot();

    void update();

    void setMoveDirectionVector(glm::vec3 direction);

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

    void draw();
    void createLegs();
    void updateLegs();

    void createBody();
    void updateBody();
    void applyBodyTransformation(glm::mat4 transformation);

    GameObject* gameObject;

    std::vector<RobotLeg*> legs;
    Animator::Joint* body;
    glm::vec3 bodyOrigin;
    Animator::Joint* eyeCover;

    glm::mat4 initialModelMatrix;
};

extern Robot *robot;