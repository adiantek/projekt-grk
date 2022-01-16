#include <list>
#include <cmath>
#include <glm/ext.hpp>

#include <Utilities/glmu.hpp>
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

    this->body = this->gameObject->getModel()->getJoint("Armature_Body");
    this->createLegs();

    GlobalEvents::on(GlobalEvents::ON_GIZMOS_RENDER, [this]() {
        // Debug Robot location
        Gizmos::cube("ROBOT_LOCATION", this->position);

        // Debug legs
        for (RobotLeg* leg : this->legs) {
            Gizmos::cube("LEG_ATT_" + leg->id, leg->attachmentPoint, glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.0f, 0.0f, 1.0f));
            Gizmos::cube("LEG_EST_" + leg->id, leg->attachmentEstimation, glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.0f, 1.0f, 0.0f));
            Gizmos::line("LEG_DIST_" + leg->id, leg->attachmentPoint, leg->attachmentEstimation, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    });
}

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

    // Random body transformation
    glm::mat4 bodyPositionTransform = glm::translate(glmu::s_random_offset(1.0f, (float)Time::time * 2.0f));
    glm::vec3 bodyRotation = glmu::s_random_offset(1.0f, (float)((Time::time + 1000.0f) * 2.0f));
    this->applyBodyTransformation(
        glm::inverse(this->body->getLocalBindTransform())
        * glm::eulerAngleXYZ(bodyRotation.x, bodyRotation.y, bodyRotation.z)
        * bodyPositionTransform
        * this->body->getLocalBindTransform()
    );

    this->gameObject->setPosition(this->position);
    this->gameObject->setRotation(this->rotation);

    this->updateLegs();

    // Draw object
    this->draw();
}

void Robot::draw() {
    this->gameObject->draw();
}

void Robot::createLegs() {
    Model* model = this->gameObject->getModel();
    glm::vec3 offset = glm::vec3(0.075f, 0.0f, 0.0f);
    glm::vec3 scale = this->gameObject->getScale();

    RobotLeg* frontLeftLeg = new RobotLeg;
    frontLeftLeg->id = 0;
    frontLeftLeg->upperJoint = model->getJoint("Armature_LegFrontLeft");
    frontLeftLeg->lowerJoint = model->getJoint("Armature_LegFrontLeftLower");
        glm::vec3 frontLeftLegVector = glm::vec3(glm::inverse(frontLeftLeg->lowerJoint->getLocalBindTransform()) * glm::vec4(glm::vec3(0.0f), 1.0f));
        frontLeftLegVector.y = 0.0f;
    frontLeftLeg->attachmentPoint = glm::vec3(glm::scale(scale) * glm::vec4(frontLeftLegVector, 1.0f)) + this->position + glm::vec3(-offset.x, offset.y, -offset.z);
    frontLeftLeg->relativeAttachmentDestination = frontLeftLeg->attachmentPoint - this->position;
    frontLeftLeg->attachmentEstimation = frontLeftLeg->attachmentPoint;
    this->legs.push_back(frontLeftLeg);

    RobotLeg* frontRightLeg = new RobotLeg;
    frontRightLeg->id = 1;
    frontRightLeg->upperJoint = model->getJoint("Armature_LegFrontRight");
    frontRightLeg->lowerJoint = model->getJoint("Armature_LegFrontRightLower");
        glm::vec3 frontRightLegVector = glm::vec3(glm::inverse(frontRightLeg->lowerJoint->getLocalBindTransform()) * glm::vec4(glm::vec3(0.0f), 1.0f));
        frontRightLegVector.y = 0.0f;
    frontRightLeg->attachmentPoint = glm::vec3(glm::scale(scale) * glm::vec4(frontRightLegVector, 1.0f)) + this->position + glm::vec3(offset.x, offset.y, -offset.z);
    frontRightLeg->relativeAttachmentDestination = frontRightLeg->attachmentPoint - this->position;
    frontRightLeg->attachmentEstimation = frontRightLeg->attachmentPoint;
    this->legs.push_back(frontRightLeg);

    RobotLeg* backLeftLeg = new RobotLeg;
    backLeftLeg->id = 2;
    backLeftLeg->upperJoint = model->getJoint("Armature_LegBackLeft");
    backLeftLeg->lowerJoint = model->getJoint("Armature_LegBackLeftLower");
        glm::vec3 backLeftLegVector = glm::vec3(glm::inverse(backLeftLeg->lowerJoint->getLocalBindTransform()) * glm::vec4(glm::vec3(0.0f), 1.0f));
        backLeftLegVector.y = 0.0f;
    backLeftLeg->attachmentPoint = glm::vec3(glm::scale(scale) * glm::vec4(backLeftLegVector, 1.0f)) + this->position + glm::vec3(-offset.x, offset.y, offset.z);
    backLeftLeg->relativeAttachmentDestination = backLeftLeg->attachmentPoint - this->position;
    backLeftLeg->attachmentEstimation = backLeftLeg->attachmentPoint;
    this->legs.push_back(backLeftLeg);

    RobotLeg* backRightLeg = new RobotLeg;
    backRightLeg->id = 3;
    backRightLeg->upperJoint = model->getJoint("Armature_LegBackRight");
    backRightLeg->lowerJoint = model->getJoint("Armature_LegBackRightLower");
        glm::vec3 backRightLegVector = glm::vec3(glm::inverse(backRightLeg->lowerJoint->getLocalBindTransform()) * glm::vec4(glm::vec3(0.0f), 1.0f));
        backRightLegVector.y = 0.0f;
    backRightLeg->attachmentPoint = glm::vec3(glm::scale(scale) * glm::vec4(backRightLegVector, 1.0f)) + this->position + glm::vec3(offset.x, offset.y, offset.z);
    backRightLeg->relativeAttachmentDestination = backRightLeg->attachmentPoint - this->position;
    backRightLeg->attachmentEstimation = backRightLeg->attachmentPoint;
    this->legs.push_back(backRightLeg);
}

void Robot::updateLegs() {
    for (RobotLeg* leg : this->legs) {
        leg->attachmentEstimation = 
            glm::translate(this->position) *
            glm::eulerAngleXYZ(
                glm::radians(this->rotation.x),
                glm::radians(this->rotation.y),
                glm::radians(this->rotation.z)
            ) * glm::vec4(leg->relativeAttachmentDestination, 1.0f);

        leg->lowerJoint->setTransform(
            glm::rotate(glm::radians(90.0f), glm::vec3(1,0,0))
            * glm::translate(
                glm::vec3(glm::scale(glmu::inverse_scale(this->gameObject->getScale()))
                * glm::eulerAngleXYZ(-glm::radians(this->rotation.x), -glm::radians(this->rotation.y), -glm::radians(this->rotation.z))
                * glm::vec4((leg->attachmentPoint - leg->attachmentEstimation), 1.0f))
            ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1,0,0))
        );

        // Legs should look at upper joints local binds
        leg->upperJoint->setTransform(
            glm::lookAt(
                glm::vec3(
                    glm::inverse(leg->upperJoint->getLocalBindTransform()) * glm::vec4(glm::vec3(0.0f), 1.0f)
                ),
                leg->attachmentPoint,
                glm::vec3(0, 1, 0)
            )
        );
    }
}

void Robot::applyBodyTransformation(glm::mat4 transformation) {
    this->body->setTransform(transformation);
}

Robot *robot;