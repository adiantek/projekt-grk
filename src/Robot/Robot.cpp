#include <list>

#include <Robot/Robot.hpp>
#include <Resources/Resources.hpp>
#include <Time/Time.hpp>
#include <Gizmos/Gizmos.hpp>

Robot::Robot() {
    robot = this;

    this->position = glm::vec3(0.0f, 0.0f, 6.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    GameObject* gameObject = new GameObject("Robot");

    this->gameObject = gameObject
        ->setModel(Resources::MODELS.ROBOT)
        ->setScale(glm::vec3(0.1f, 0.1f, 0.1f))
        ->setMaterials({
            Resources::MATERIALS.ROBOT_BODY,
            Resources::MATERIALS.ROBOT_METAL,
            Resources::MATERIALS.ROBOT_CHROME,
            Resources::MATERIALS.ROBOT_LENS,
            Resources::MATERIALS.ROBOT_WIRE,
            Resources::MATERIALS.ROBOT_GLOW,
            Resources::MATERIALS.ROBOT_METAL_2,
            Resources::MATERIALS.ROBOT_METAL_PAINTED
        });

    this->mode = Robot::MODE_STATIONARY;
    this->movementSpeed = Robot::DEFAULT_WALKING_MOVEMENT_SPEED;
}

// void Robot::toggleIncreasedSpeedMode() {
//     if (this->movementSpeed == Robot::MAX_SPEED_INCREASED) {
//         this->movementSpeed = Robot::DEFAULT_MOVEMENT_SPEED;
//     } else {
//         this->movementSpeed = Robot::MAX_SPEED_INCREASED;
//     }
// }

void Robot::enableIncreasedSpeedMode() {
    if( this->mode == Robot::MODE_WALKING) {
        this->movementSpeed = Robot::MAX_WALKING_SPEED_INCREASED;
    } else {
        this->movementSpeed = Robot::MAX_SWIMMING_SPEED_INCREASED;
    }
}

void Robot::disableIncreasedSpeedMode() {
    if( this->mode == Robot::MODE_WALKING) {
        this->movementSpeed = Robot::DEFAULT_WALKING_MOVEMENT_SPEED;
    } else {
        this->movementSpeed = Robot::DEFAULT_SWIMMING_MOVEMENT_SPEED;
    }
}

void Robot::setMoveDirectionVector(glm::vec3 direction) {
    if (direction.x == 0 && direction.y == 0 && direction.z == 0) {
        this->mode = Robot::MODE_STATIONARY;
    } else {
        this->mode = Robot::MODE_WALKING;
    }

    this->moveDirectionVector = glm::normalize(direction);
}

void Robot::update() {
    // Update position
    if (this->mode == Robot::MODE_WALKING) {
        this->position += this->moveDirectionVector * this->movementSpeed * timeExternal->deltaTime;

        // Iterpolate rotation to direction vector
        float rotationSpeed = Robot::ROTATION_SPEED * timeExternal->deltaTime * 5.0f;
        // this->rotation += glm::normalize(rotationSpeed * this->rotation + (1 - rotationSpeed) * this->moveDirectionVector);
        this->rotation = this->moveDirectionVector;
    }

    // Draw Gizmos for testing purposes
    Gizmos::cube(this->position);

    this->gameObject->setPosition(this->position);
    this->gameObject->setRotation(this->rotation);

    // Draw object
    this->draw();
}

void Robot::draw() {
    this->gameObject->draw();
}

Robot *robot;