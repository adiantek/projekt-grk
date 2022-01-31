#include <list>
#include <stdbool.h>
#include <cmath>

#include <Robot/Robot.hpp>
#include <Resources/Resources.hpp>
#include <ResourceLoader.hpp>
#include <Time/Time.hpp>
#include <utils/glmu.hpp>
#include <Logger.h>
#include <utils/Gizmos.hpp>
#include <Physics/Physics.hpp>

using namespace utils;
using namespace entity;

Robot::Robot() {
    robot = this;

    this->position = glm::vec3(0.0f, 130.0f, -64.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 0.0f);

    GameObject* gameObject = new GameObject("Robot");

    this->gameObject = gameObject
        ->setModel(resourceLoaderExternal->m_robot_with_armature)
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

    this->position = this->getWorldPointAt(this->position);

    this->initialPosition = glm::vec3(this->position);
    this->initialModelMatrix = this->gameObject->getModelMatrix();
    this->createBody();
    this->createLegs();

    bool debug = true;

    if (debug) {
        Gizmos::onDraw([this]() {
            // Debug Robot location
            Gizmos::cube(this->position);

            // Debug Body location
            Gizmos::cube(
                this->gameObject->getModelMatrix() * glm::vec4(this->bodyOrigin, 1.0f),
                glm::vec3(0.02f, 0.02f, 0.02f),
                glm::vec3(1.0f, 1.0f, 1.0f)
            );

            // Debug up vector
            Gizmos::line(
                this->gameObject->getModelMatrix() * glm::vec4(glm::vec3(0.0f), 1.0f),
                this->gameObject->getModelMatrix() * glm::vec4(this->up, 1.0f),
                glm::vec3(1.0f, 0.0f, 0.0f)
            );

            // Debug forward vector
            Gizmos::line(
                this->gameObject->getModelMatrix() * glm::vec4(glm::vec3(0.0f), 1.0f),
                this->gameObject->getModelMatrix() * glm::vec4(this->forward, 1.0f),
                glm::vec3(1.0f, 1.0f, 0.0f)
            );

            // Debug legs
            for (RobotLeg* leg : this->legs) {
                Gizmos::cube(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->upperJointOrigin, 1.0f),
                    glm::vec3(0.02f, 0.02f, 0.02f),
                    glm::vec3(0.0f, 0.0f, 1.0f)
                );

                Gizmos::cube(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->lowerJointOrigin, 1.0f),
                    glm::vec3(0.02f, 0.02f, 0.02f),
                    glm::vec3(1.0f, 1.0f, 0.0f)
                );

                Gizmos::line(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->upperJointOrigin, 1.0f),
                    this->gameObject->getModelMatrix() * glm::vec4(this->bodyOrigin, 1.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f)
                );

                Gizmos::line(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->upperJointOrigin, 1.0f),
                    this->gameObject->getModelMatrix() * glm::vec4(leg->lowerJointOrigin, 1.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f)
                );

                Gizmos::cube(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->globalAttachmentPoint, 1.0f),
                    glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(1.0f, 1.0f, 1.0f)
                );

                Gizmos::cube(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->attachmentEstimation, 1.0f),
                    glm::vec3(0.02f, 0.02f, 0.02f),
                    glm::vec3(0.0f, 1.0f, 0.0f)
                );

                Gizmos::line(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->globalAttachmentPoint, 1.0f),
                    this->gameObject->getModelMatrix() * glm::vec4(leg->attachmentEstimation, 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)
                );

                Gizmos::line(
                    this->gameObject->getModelMatrix() * glm::vec4(leg->lowerJointOrigin, 1.0f),
                    this->gameObject->getModelMatrix() * glm::vec4(leg->globalAttachmentPoint, 1.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)
                );
            }
        });
    }
}

Robot::~Robot() {
    // TODO oczyscic robota
}

void Robot::enableIncreasedSpeedMode() {
    if (this->mode == Robot::MODE_WALKING) {
        this->movementSpeed = Robot::MAX_WALKING_SPEED_INCREASED;
    } else {
        this->movementSpeed = Robot::MAX_SWIMMING_SPEED_INCREASED;
    }
}

void Robot::disableIncreasedSpeedMode() {
    if (this->mode == Robot::MODE_WALKING) {
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

    this->position = this->getWorldPointAt(this->position);

    this->gameObject->setPosition(this->position);
    this->gameObject->setRotation(this->rotation);

    this->updateBody();
    this->updateLegs();
    this->updateDirections();
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
    bool ignoreAnimation = false;

    if (this->movementSpeed >= MAX_WALKING_SPEED_INCREASED) {
        ignoreAnimation = true;
    }

    this->bodyOrigin = glm::inverse(this->body->getLocalBindTransform()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 backCenter = (this->legs[0]->globalAttachmentPoint + this->legs[1]->globalAttachmentPoint) / 2;
    glm::vec3 frontCenter = (this->legs[2]->globalAttachmentPoint + this->legs[3]->globalAttachmentPoint) / 2;
    glm::vec3 center = (backCenter + frontCenter) / 2;
    center.z = 0;

    glm::vec3 leftCenter = (this->legs[0]->globalAttachmentPoint + this->legs[2]->globalAttachmentPoint) / 2;
    glm::vec3 rightCenter = (this->legs[1]->globalAttachmentPoint + this->legs[3]->globalAttachmentPoint) / 2;

    // Random body transformation
    glm::vec3 currentPos = this->body->getTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 deltaPos = center - currentPos;
    glm::vec3 newPos = currentPos + (deltaPos * 0.01f);

    // Swing and tilt rotation
    glm::quat swingQuat = glm::rotationCamera(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(backCenter - frontCenter));
    glm::quat tiltQuat = glm::rotationCamera(glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(rightCenter - leftCenter));

    glm::quat rotationQuat = swingQuat * tiltQuat;

    if (glm::any(glm::isnan(rotationQuat)) || glm::any(glm::isnan(this->bodyRotation)) || ignoreAnimation) {
        this->bodyRotation = swingQuat * tiltQuat;
    } else {
        this->bodyRotation = glm::slerp(this->bodyRotation, swingQuat * tiltQuat, timeExternal->deltaTime * 2.0f);
    }
    
    glm::mat4 bodyPositionTransform = glm::translate(newPos);

    this->applyBodyTransformation(
        glm::mat4_cast(this->bodyRotation)
    );
}

void Robot::createLegs() {
    Model* model = this->gameObject->getModel();
    glm::vec3 scale = this->gameObject->getScale();
    glm::vec3 offset = glm::vec3(0.7f, 0.0f, 0.0f);
    glm::vec3 targetOffset = glm::vec3(0.3f, 0.0f, 0.0f);

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
            legVector.z = 0.0f;
        glm::mat4 localOffset = (leg->lowerJoint->getOrigin().x > leg->upperJoint->getOrigin().x) ?
            glm::translate(offset) : glm::inverse(glm::translate(offset));
        glm::mat4 localTargetOffset = (leg->lowerJoint->getOrigin().x > leg->upperJoint->getOrigin().x) ?
            glm::translate(targetOffset) : glm::inverse(glm::translate(targetOffset));

        // This I am not pretty sure, but I think is ok
        leg->lowerJointLengthVector = localOffset * glm::vec4(leg->lowerJoint->getOrigin() - legVector, 1.0f);

        // This two lengths are pretty accurate
        leg->upperJointLength = glm::distance(leg->upperJoint->getOrigin(), leg->lowerJoint->getOrigin());
        leg->lowerJointLength = glm::length(leg->lowerJointLengthVector);

        // calculate lower leg part joint origin
        glm::vec3 lowerJointOrigin = leg->lowerJoint->getOrigin();
        lowerJointOrigin.z = 0.0f;

        // Zigzag offset -> define what offset should be between opposite legs
        float zigzagOffset = 0.5f * (leg->id == 1 || leg->id == 2);

        // calculate current attachment point in world space
        leg->attachmentPoint = localTargetOffset * glm::vec4(lowerJointOrigin + glm::vec3(0.0f, zigzagOffset, 0.0f), 1.0f);
    }
}

void Robot::updateLegs() {
    bool ignoreAnimation = false;

    if (this->movementSpeed >= MAX_WALKING_SPEED_INCREASED) {
        ignoreAnimation = true;
    }

    glm::mat4 modelMatrix = this->gameObject->getModelMatrix();
    glm::mat4 invertedModelMatrix = glm::inverse(modelMatrix)
        * (glm::translate(this->initialPosition)
        * this->initialModelMatrix);

    glm::vec3 offset = glm::vec3(0.7f, 0.0f, 0.0f);
    glm::vec3 targetOffset = glm::vec3(0.3f, 0.0f, 0.0f);

    float longestStep = 0.0f;
    RobotLeg* legToMove = this->legs[0];

    for (RobotLeg* leg : this->legs) {
        glm::vec3 lowerJointOrigin = leg->lowerJoint->getOrigin();
        lowerJointOrigin.z = 0.0f;
        glm::mat4 localOffset = (leg->lowerJoint->getOrigin().x > leg->upperJoint->getOrigin().x) ?
            glm::translate(offset) : glm::inverse(glm::translate(offset));
        glm::mat4 localTargetOffset = (leg->lowerJoint->getOrigin().x > leg->upperJoint->getOrigin().x) ?
            glm::translate(targetOffset) : glm::inverse(glm::translate(targetOffset));

         // Calculate attachment point estimation
        leg->attachmentEstimation = localTargetOffset * glm::vec4(lowerJointOrigin, 1.0f);
        leg->attachmentEstimation.y += 1.0f * (leg->id < 2);
        leg->attachmentEstimation = glm::inverse(modelMatrix) * glm::vec4(this->getWorldPointAt(glm::vec3(modelMatrix * glm::vec4(leg->attachmentEstimation, 1.0f))), 1.0f);
        glm::vec3 globalAttachmentEstimation = glm::vec3(modelMatrix * glm::vec4(leg->attachmentEstimation, 1.0f));
        glm::vec3 globalAttachmentPoint = glm::vec3(invertedModelMatrix * glm::vec4(leg->attachmentPoint, 1.0f));
        leg->globalAttachmentPoint = globalAttachmentPoint;

        // Calculate circle midpoint between attachment point and upper leg joint origin
        glm::vec2 attachmentPositionFlat = glm::vec2(globalAttachmentPoint.x, globalAttachmentPoint.z);
        glm::vec2 upperJointOriginFlat = glm::vec2(leg->upperJointOrigin.x, leg->upperJointOrigin.z);
        glm::vec3 midPoint = utils::glmu::circles_midpoint(globalAttachmentPoint, leg->upperJointOrigin, leg->lowerJointLength, leg->upperJointLength, this->up);
        // glm::vec3 midPoint = glm::vec3(midPointFlat.x, (globalAttachmentPoint.y + leg->upperJointOrigin.y) / 2, midPointFlat.y);
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
        glm::vec3 lNewDirectionVector = glm::normalize(globalAttachmentPoint - leg->lowerJointOrigin);
        glm::quat lQuaternionRotation = glm::rotationCamera(lCurrentDirectionVector, lNewDirectionVector);

        leg->lowerJoint->setTransform(
            glm::translate(leg->lowerJointOrigin)
            * glm::mat4_cast(lQuaternionRotation)
            * glm::inverse(glm::translate(leg->lowerJoint->getOrigin()))
        );

        float distanceSquare = glm::distance2(leg->globalAttachmentPoint, leg->attachmentEstimation);

        if (distanceSquare > Robot::LEG_MAX_DISTANCE_SQUARE && leg->step < 0.0f) {
            bool isStationary = true;
            int currentLegId = leg->id;

            for (RobotLeg* leg : this->legs) {
                bool isOpposite = false;

                if (currentLegId == 0 && leg->id == 1) {
                    isOpposite = true;
                } else if (currentLegId == 1 && leg->id == 2) {
                    isOpposite = true;
                } else if (currentLegId == 2 && leg->id == 1) {
                    isOpposite = true;
                } else if (currentLegId == 3 && leg->id == 0) {
                    isOpposite = true;
                }

                float otherLegDistance = glm::distance2(leg->globalAttachmentPoint, leg->attachmentEstimation);

                if (!isOpposite && leg->step > 0.0f && otherLegDistance < distanceSquare) {
                    isStationary = false;
                    break;
                }
            }

            if (isStationary) {
                leg->previousAttachmentPoint = glm::vec3(leg->attachmentPoint);
                leg->targetAttachmentPoint = glm::vec3(glm::inverse(invertedModelMatrix) * glm::vec4(leg->attachmentEstimation, 1.0f));
                leg->step = 0.0f;
                leg->currentStepDistance = distanceSquare;
            } 
        }

        if (ignoreAnimation) {
            leg->attachmentPoint = leg->targetAttachmentPoint;
            leg->step = -1.0f;
        } else {
            if (leg->step >= 0.0f && leg->step < 1.0f) {
                leg->attachmentPoint = glmu::curve(leg->previousAttachmentPoint, leg->targetAttachmentPoint, leg->step);
                leg->step += Robot::LEG_STEP_SPEED * timeExternal->deltaTime * (leg->currentStepDistance*2);
            }

            if (leg->step > 1.0f) {
                leg->attachmentPoint = leg->targetAttachmentPoint;
                leg->step = -1.0f;
            }
        }
    }


}

void Robot::jump() {
    this->mode = Robot::MODE_JUMPING;

    // TODO: Rest of jumping logic
}

void Robot::updateDirections() {
    glm::vec3 _sideways = glm::normalize(this->legs[0]->upperJointOrigin - this->legs[1]->upperJointOrigin);
    glm::vec3 _forward = glm::normalize(this->legs[0]->upperJointOrigin - this->legs[2]->upperJointOrigin);
    glm::vec3 _up = -glm::normalize(glm::cross(_sideways, _forward));

    this->forward = _forward;
    this->up = _up;
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

glm::vec3 Robot::getWorldPointAt(glm::vec3 point) {
    glm::vec3 result = glm::vec3(point);
    result.y = worldObject->getHeightAt(point.x, point.z);

    if (isnan(result.y)) {
        result.y = point.y;
        return result;
    }

    // If robot would like 
    // float maxSlope = 0.5f;

    // physx::PxVec3 origin = physx::PxVec3(point.x, std::max(result.y + maxSlope, point.y + maxSlope), point.z);
    // physx::PxVec3 unitDir = physx::PxVec3(0, -1.0f, 0);
    // physx::PxReal maxDistance = 100;
    // physx::PxRaycastBuffer hit;
    
    // bool status = physicsObject->scene->raycast(origin, unitDir, maxDistance, hit);
    // if (status) {
    //     std::cout <<  "Raycast hit at " << hit.block.position.x << ", " << hit.block.position.y << ", " << hit.block.position.z << std::endl;
    //     result.y = hit.block.position.y;
    // }

    return result;
}

Robot *robot;