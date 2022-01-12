#include <list>

#include <Robot/Robot.hpp>
#include <Resources/Resources.hpp>
#include <Time/Time.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Resources/GlobalEvents.hpp>

Robot::Robot() {
    robot = this;

    this->position = glm::vec3(0.0f, 0.0f, 6.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 0.0f);

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

    GlobalEvents::on(GlobalEvents::ON_GIZMOS_RENDER, [this]() {
        // Gizmos::line(this->position, this->position + glm::vec3(0.0f, 2.0f, 0.0f));
    });
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
    if (this->mode != Robot::MODE_STATIONARY) {
        this->position += this->moveDirectionVector * this->movementSpeed * timeExternal->deltaTime;

        if (this->moveDirectionVector.x != 0.0f || this->moveDirectionVector.z != 0.0f) {
            float rotationSpeed = Robot::ROTATION_SPEED * timeExternal->deltaTime * 5.0f;

            this->moveDirectionVector.y = 0.0f;
            this->moveDirectionVector = glm::normalize(this->moveDirectionVector);

            float rotationAngle = glm::orientedAngle(glm::vec3(0, 0, -1), this->moveDirectionVector, glm::vec3(0, 1, 0)) / glm::pi<float>() * 180.0f;
            // float rotationAngle = glm::orientedAngle(glm::vec3(0, 0, -1), iterpolatedVector, glm::vec3(0, 1, 0)) / glm::pi<float>() * 180.0f;

            if(rotation.y > 360) {
                rotation.y -= 360;
            } else if(rotation.y < 0) {
                rotation.y += 360;
            }
            if(rotationAngle > 360) {
                rotationAngle -= 360;
            } else if(rotationAngle < 0) {
                rotationAngle += 360;
            }

            if (abs(rotationAngle - this->rotation.y) < rotationSpeed) {
                this->rotation.y = rotationAngle;
            } else if ((this->rotation.y > rotationAngle && this->rotation.y < rotationAngle + 180.0f) || this->rotation.y < rotationAngle - 180.0f) {
                this->rotation.y -= rotationSpeed;
            } else {
                this->rotation.y += rotationSpeed;
            }
        }

        
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