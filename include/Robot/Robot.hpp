#pragma once

#include <stdbool.h>

#include <Resources/GameObject.hpp>
#include <glm/ext.hpp>
#include <world/Object3D.hpp>

namespace entity {

struct RobotLeg {
    int id;

    Animator::Joint* upperJoint;
    Animator::Joint* lowerJoint;

    float upperJointLength = 0.0f;
    float lowerJointLength = 0.0f;

    /* This is the actual point attached to the leg (when move, bones are also moving) */
    glm::vec3 attachmentPoint = glm::vec3(0.0f);
    glm::vec3 globalAttachmentPoint = glm::vec3(0.0f);

    glm::vec3 attachmentEstimation = glm::vec3(0.0f);
    glm::vec3 targetAttachmentPoint = glm::vec3(0.0f);
    glm::vec3 previousAttachmentPoint = glm::vec3(0.0f);
    float step = -1.0f;
    float currentStepDistance = 0.0f;

    glm::vec3 lowerJointLengthVector = glm::vec3(0.0f);

    glm::vec3 upperJointOrigin = glm::vec3(0.0f);
    glm::vec3 lowerJointOrigin = glm::vec3(0.0f);
};

class Robot : world::Object3D {
   public:
    // SPEEDS
    inline static const float DEFAULT_SWIMMING_MOVEMENT_SPEED = 4.0f;
    inline static const float MAX_SWIMMING_SPEED_INCREASED = 8.0f * 10;

    inline static const float DEFAULT_WALKING_MOVEMENT_SPEED = 0.5f;
    inline static const float MAX_WALKING_SPEED_INCREASED = 1.5f * 10;

    inline static const float ROTATION_SPEED = 15.0f;
    inline static const float LEG_STEP_SPEED = 7.0f;
    inline static const float LEG_MAX_DISTANCE_SQUARE = 0.6f;
    inline static const float JUMP_SPEED = 1.0f;
    inline static const float LANDING_SPEED = 1.5f;

    inline static const float JUMP_HEIGHT = 10.0f;

    // MODES
    static const int MODE_STATIONARY = 0;           // When the robot is not moving and touching the ground
    static const int MODE_STATIONARY_FLOATING = 1;  // When robot is not moving, but floating in the water
    static const int MODE_WALKING = 2;              // When robot is walking on the ground
    static const int MODE_SWIMMING = 3;             // When robot is swimming in the water
    static const int MODE_SLEEPING = 4;             // When robot is stationary for long time
    static const int MODE_JUMPING = 5;              // When robot is trying to leave the ground
    static const int MODE_LANDING = 6;              // When robot is landing on the ground

    float currentSpeed = 0;
    float currentTurnSpeed = 0;

    Robot();
    virtual ~Robot();

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;

    void setMoveDirectionVector(glm::vec3 direction);

    void enableIncreasedSpeedMode();
    void disableIncreasedSpeedMode();

    void jump();
    void land();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 direction;

   private:
    glm::vec3 moveDirectionVector;

    int mode = Robot::MODE_STATIONARY;
    float movementSpeed;
    float movementVector;
    void createLegs();
    void updateLegs();

    void createBody();
    void updateBody();
    void updateDirections();
    void applyBodyTransformation(glm::mat4 transformation);
    bool isInGroundMode();
    bool isInFloatingMode();
    bool isInMovingMode();
    float getDistanceFromGround();
    glm::vec3 getWorldPointAt(glm::vec3 point);

    GameObject* gameObject;

    std::vector<RobotLeg*> legs;
    Animator::Joint* body;
    glm::vec3 bodyOrigin;
    glm::quat bodyRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    Animator::Joint* eyeCover;

    glm::vec3 jumpStart = glm::vec3(0.0f);
    glm::vec3 jumpTarget = glm::vec3(0.0f);
    float jumpStage = 0.0f;

    glm::vec3 landStart = glm::vec3(0.0f);
    glm::vec3 landTarget = glm::vec3(0.0f);
    float landStage = 0.0f;

    glm::mat4 initialModelMatrix;
    glm::vec3 initialPosition;
    glm::vec3 up;
    glm::vec3 forward;
};

}  // namespace entity

extern entity::Robot *robot;