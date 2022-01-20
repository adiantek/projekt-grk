#include <list>

#include <Robot/Robot.hpp>
#include <Resources/Resources.hpp>
#include <Time/Time.hpp>
#include <utils/glmu.hpp>
#include <Logger.h>

using namespace entity;

Robot::Robot() {
    robot = this;

    this->position = glm::vec3(8.0f, 128.0f, 8.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 0.0f);

    GameObject* gameObject = new GameObject("Robot");

    this->gameObject = gameObject
        ->setModel(Resources::MODELS.ROBOT)
        ->setScale(glm::vec3(0.3f, 0.3f, 0.3f))
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

    this->initialModelMatrix = this->gameObject->getModelMatrix();
    this->createBody();
    this->createLegs();
}

Robot::~Robot() {
    // TODO oczyscic robota
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
        this->mode = Robot::MODE_SWIMMING;
    }

    this->moveDirectionVector = glm::normalize(direction);
}

void Robot::update() {
    float rotationSpeed = Robot::ROTATION_SPEED * timeExternal->deltaTime * 5.0f;

    if (this->mode != Robot::MODE_STATIONARY) {
        this->position += this->moveDirectionVector * this->movementSpeed * timeExternal->deltaTime;

        if (this->mode == Robot::MODE_SWIMMING) {
            float rotationAngleX = 0.0f;
            if (this->moveDirectionVector.y > 0.0f) {
                rotationAngleX = 45.0f;
            } else if (this->moveDirectionVector.y < 0.0f) {
                rotationAngleX = -45.0f;
            }

            if (abs(rotationAngleX - this->rotation.x) < rotationSpeed) {
                this->rotation.x = rotationAngleX;
            } else if (this->rotation.x > rotationAngleX ) {
                this->rotation.x -= rotationSpeed;
            } else {
                this->rotation.x += rotationSpeed;
            }

            // else {
            //     rotaionAngleX = 0.0f;
            // }
        } else if (this->mode == Robot::MODE_WALKING) {
            // rotation x for walking
        }

        if (this->moveDirectionVector.x != 0.0f || this->moveDirectionVector.z != 0.0f) {

            this->moveDirectionVector.y = 0.0f;
            this->moveDirectionVector = glm::normalize(this->moveDirectionVector);

            float rotationAngle = glm::orientedAngle(glm::vec3(0, 0, -1), this->moveDirectionVector, glm::vec3(0, 1, 0)) / glm::pi<float>() * 180.0f;
            // float rotationAngle = glm::orientedAngle(glm::vec3(0, 0, -1), iterpolatedVector, glm::vec3(0, 1, 0)) / glm::pi<float>() * 180.0f;

            if (rotation.y > 360) {
                rotation.y -= 360;
            } else if (rotation.y < 0) {
                rotation.y += 360;
            }
            if (rotationAngle > 360) {
                rotationAngle -= 360;
            } else if (rotationAngle < 0) {
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
    } else {
        float rotationAngleX = 0.0f;
        if (abs(rotationAngleX - this->rotation.x) < rotationSpeed) {
            this->rotation.x = rotationAngleX;
        } else if (this->rotation.x > rotationAngleX ) {
            this->rotation.x -= rotationSpeed;
        } else {
            this->rotation.x += rotationSpeed;
        }
    }

    this->gameObject->setPosition(this->position);
    this->gameObject->setRotation(this->rotation);

    this->updateBody();
    this->updateLegs();
}

void Robot::draw(glm::mat4 mat) {
    this->gameObject->draw(mat);
}

void Robot::drawShadow(glm::mat4 mat) {
    this->gameObject->drawShadow(mat);
}


void Robot::createBody() {
    this->body = this->gameObject->getModel()->getJoint("Armature_Body");
    this->eyeCover = this->gameObject->getModel()->getJoint("Armature_EyeCover");
}

void Robot::updateBody() {
    this->bodyOrigin = glm::inverse(this->body->getLocalBindTransform()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Random body transformation
    glm::mat4 bodyPositionTransform = glm::translate(utils::s_random_offset(0.3f, (float)timeExternal->lastFrame * 5.0f));
    glm::vec3 bodyRotation = utils::s_random_offset(10.0f, (float)((timeExternal->lastFrame + 1000.0f) * 5.0f));

    this->applyBodyTransformation(
        glm::translate(this->bodyOrigin)
        * bodyPositionTransform
        * glm::eulerAngleXYZ(glm::radians(bodyRotation.x), glm::radians(bodyRotation.y), glm::radians(bodyRotation.z))
        * glm::translate(glm::vec3(this->body->getLocalBindTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)))
    );
}

void Robot::createLegs() {
    Model* model = this->gameObject->getModel();
    glm::vec3 scale = this->gameObject->getScale();
    glm::vec3 offset = glm::vec3(0.7f, 0.0f, 0.0f);

    glm::mat4 modelMatrix = this->gameObject->getModelMatrix();

    RobotLeg* frontLeftLeg = new RobotLeg;
    frontLeftLeg->id = 0;
    frontLeftLeg->upperJoint = model->getJoint("Armature_LegFrontLeft");
    frontLeftLeg->lowerJoint = model->getJoint("Armature_LegFrontLeftLower");
    this->legs.push_back(frontLeftLeg);

    RobotLeg* frontRightLeg = new RobotLeg;
    frontRightLeg->id = 1;
    frontRightLeg->upperJoint = model->getJoint("Armature_LegFrontRight");
    frontRightLeg->lowerJoint = model->getJoint("Armature_LegFrontRightLower");
    this->legs.push_back(frontRightLeg);

    RobotLeg* backLeftLeg = new RobotLeg;
    backLeftLeg->id = 2;
    backLeftLeg->upperJoint = model->getJoint("Armature_LegBackLeft");
    backLeftLeg->lowerJoint = model->getJoint("Armature_LegBackLeftLower");
    this->legs.push_back(backLeftLeg);

    RobotLeg* backRightLeg = new RobotLeg;
    backRightLeg->id = 3;
    backRightLeg->upperJoint = model->getJoint("Armature_LegBackRight");
    backRightLeg->lowerJoint = model->getJoint("Armature_LegBackRightLower");
    this->legs.push_back(backRightLeg);

    for (RobotLeg* leg : this->legs) {
        glm::vec3 legVector = leg->lowerJoint->getOrigin();
        glm::mat4 localOffset = (leg->lowerJoint->getOrigin().x < leg->upperJoint->getOrigin().x) ?
            glm::translate(offset) : glm::inverse(glm::translate(offset));
        legVector.z = 0.0f;

        // This I am not pretty sure, but I think is ok
        leg->lowerJointLengthVector = localOffset * glm::vec4(leg->lowerJoint->getOrigin() - legVector, 1.0f);

        // This two lengths are pretty accurate
        leg->upperJointLength = glm::distance(leg->upperJoint->getOrigin(), leg->lowerJoint->getOrigin());
        leg->lowerJointLength = glm::length(leg->lowerJointLengthVector);
    }
}

void Robot::updateLegs() {
    glm::mat4 modelMatrix = this->gameObject->getModelMatrix();
    glm::mat4 invertedModelMatrix = glm::inverse(modelMatrix)
        * (glm::translate(glm::vec3(0.0f, 0.0f, 6.0f))
        * this->initialModelMatrix);

    //! Need to be tweaked, legs are not on that point
    glm::vec3 offset = glm::vec3(0.55f, 0.0f, 0.0f);

    for (RobotLeg* leg : this->legs) {

        // Zigzag offset is good
        float zigzagOffset = 0.5f * (leg->id == 1 || leg->id == 2);

        // This works good at first sight
        glm::vec3 lowerJointOrigin = leg->lowerJoint->getOrigin();
        lowerJointOrigin.z = 0.0f;
        glm::mat4 localOffset = (leg->lowerJoint->getOrigin().x > leg->upperJoint->getOrigin().x) ?
            glm::translate(offset) : glm::inverse(glm::translate(offset));

        // This should be runned only at the start and I thinks this works well
        if (leg->currentAttachmentPoint.x == -1.0f) {
            LOGD("Attachment point: %d", leg->id);
            leg->currentAttachmentPoint = localOffset * glm::vec4(lowerJointOrigin + glm::vec3(0.0f, zigzagOffset, 0.0f), 1.0f);
        }

        leg->attachmentPoint = glm::vec3(invertedModelMatrix * glm::vec4(leg->currentAttachmentPoint, 1.0f));
        leg->attachmentEstimation = localOffset * glm::vec4(lowerJointOrigin, 1.0f);

        // Calculate circle midpoint between attachment point and upper leg joint origin
        glm::vec2 attachmentPositionFlat = glm::vec2(leg->attachmentPoint.x, leg->attachmentPoint.z);
        glm::vec2 upperJointOriginFlat = glm::vec2(leg->upperJointOrigin.x, leg->upperJointOrigin.z);
        glm::vec2 midPointFlat = utils::circles_midpoint(attachmentPositionFlat, upperJointOriginFlat, leg->lowerJointLength, leg->upperJointLength);
        // LOGD("Midpoint: %f", leg->attachmentPoint.y);
        glm::vec3 midPoint = glm::vec3(midPointFlat.x, (leg->attachmentPoint.y + leg->upperJointOrigin.y) / 2, midPointFlat.y);
        leg->lowerJointOrigin = midPoint;

        // Calculate upper joint rotation and position
        glm::vec3 currentDirectionVector = glm::normalize(leg->lowerJoint->getOrigin() - leg->upperJoint->getOrigin());
        glm::vec3 newDirectionVector = glm::normalize(leg->lowerJointOrigin - leg->upperJointOrigin);
        glm::quat quaternionRotation = glm::rotationCamera(currentDirectionVector, newDirectionVector);

        leg->upperJoint->setTransform(
            glm::translate(leg->upperJointOrigin)
            * glm::mat4_cast(quaternionRotation)
            * glm::inverse(glm::translate(leg->upperJoint->getOrigin()))
        );

        glm::vec3 lCurrentDirectionVector = glm::normalize(glm::vec3(localOffset * glm::vec4(lowerJointOrigin, 1.0f)) - leg->lowerJoint->getOrigin());
        glm::vec3 lNewDirectionVector = glm::normalize(leg->attachmentPoint - leg->lowerJointOrigin);
        glm::quat lQuaternionRotation = glm::rotationCamera(lCurrentDirectionVector, lNewDirectionVector);


        //! This works really bad
        // The vector should be transformed by the upper joint transform first
        // Next from this vector we could actually calculate quaternion rotation
        leg->lowerJoint->setTransform(
            // glm::translate(leg->attachmentPoint)
            glm::translate(leg->lowerJointOrigin)
            * glm::mat4_cast(lQuaternionRotation)
            // * glm::translate(leg->lowerJointLengthVector)
            * glm::inverse(glm::translate(leg->lowerJoint->getOrigin()))
        );

        float distanceSquare = glm::distance2(leg->attachmentPoint, leg->attachmentEstimation);


        if (distanceSquare > Robot::LEG_MAX_DISTANCE_SQUARE && leg->step == -1.0f) {
            LOGD("Distance: %f", distanceSquare);
            leg->targetAttachmentPoint = glm::vec3(leg->attachmentEstimation.x, leg->attachmentEstimation.y, leg->attachmentEstimation.z);
            leg->previousAttachmentPoint = glm::vec3(leg->currentAttachmentPoint.x, leg->currentAttachmentPoint.y, leg->currentAttachmentPoint.z);
            leg->step = 0.0f;
        }

        if (leg->step >= 0.0f) {
            // LOGD("leg->step: %f", leg->step);
            leg->step += timeExternal->deltaTime * Robot::LEG_STEP_SPEED;
            leg->currentAttachmentPoint = utils::curve(leg->previousAttachmentPoint, leg->targetAttachmentPoint, leg->step);
        }

        if (leg->step >= 1.0f) {
            leg->step = -1.0f;
            glm::vec3 modelAttachmentPoint = modelMatrix * glm::vec4(leg->attachmentPoint, 1.0f);
            leg->currentAttachmentPoint = glm::vec3(modelAttachmentPoint.x, modelAttachmentPoint.y, leg->attachmentPoint.z);
        }
    }
}

void Robot::applyBodyTransformation(glm::mat4 transformation) {

    // This two lines works pretty well I think - body is successfully transformed
    this->body->setTransform(transformation);
    this->eyeCover->setTransform(transformation);

    for (RobotLeg* leg : this->legs) {
        // Upper joint origin is in the right place (object space)
        leg->upperJointOrigin = transformation * glm::vec4(leg->upperJoint->getOrigin(), 1.0f);
    }
}


Robot *robot;