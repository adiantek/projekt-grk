#include <Robot/Robot.hpp>
#include <Resources/Resources.hpp>
#include <Time/Time.hpp>

Robot::Robot() {
    robot = this;

    this->position = glm::vec3(0.0f, 0.0f, 6.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    GameObject* gameObject = new GameObject("Robot");

    this->gameObject = gameObject
        ->setModel(Resources::MODELS.ROBOT);
        // ->setMaterials(std::list<Material>({
        //     Resources::MATERIALS.ROBOT_BODY,
        //     Resources::MATERIALS.ROBOT_METAL
        // }));


    this->mode = Robot::MODE_STATIONARY;
    this->movementSpeed = Robot::DEFAULT_MOVEMENT_SPEED;
}

void Robot::toggleIncreasedSpeedMode() {
    if (this->movementSpeed == Robot::MAX_SPEED_INCREASED) {
        this->movementSpeed = Robot::DEFAULT_MOVEMENT_SPEED;
    } else {
        this->movementSpeed = Robot::MAX_SPEED_INCREASED;
    }
}

void Robot::enableIncreasedSpeedMode() {
    this->movementSpeed = Robot::MAX_SPEED_INCREASED;
}

void Robot::disableIncreasedSpeedMode() {
    this->movementSpeed = Robot::MAX_SPEED_INCREASED;
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
    }

    this->gameObject->setPosition(this->position);

    // Draw object
    this->draw();
}

void Robot::draw() {
    this->gameObject->draw();
}

Robot *robot;