#include <opengl.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include <Sound.hpp>
#include <Camera/Camera.hpp>
#include <Controller/Controller.hpp>
#include <Robot/Robot.hpp>
#include <Fog/Fog.hpp>
#include <Time/Time.hpp>
#include <Glow/GlowShader.hpp>
#include <Physics/Physics.hpp>
#include <world/World.hpp>

Camera::Camera(int width, int height, float fov, float near, float far, int x, int y) {
    camera = this;

    this->width = width;
    this->height = height;
    this->fov = fov; // fov = field of view
    this->near = near;
    this->far = far;
    this->x = x;
    this->y = y;

    updatePerspective();
}

Camera::~Camera() {}

glm::mat4 Camera::getTransformationMatrix() {
    return this->perspective * this->rotationMatrix;
}

glm::mat4 Camera::getCameraRotation() {
    return this->rotationMatrix;
}

void Camera::useCameraViewport() {
    glViewport(x, y, width, height);
}

void Camera::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    updatePerspective();
    
    if (Glow::glow) {
        Glow::glow->resize(width, height);
    }
    if (fog) {
        delete fog;
        fog = new Fog(width, height, 256.0);
    }
}

void Camera::setFov(float fov) {
    this->fov = fov;
    updatePerspective();
}

void Camera::mouseMove(double deltaX, double deltaY) {
    anglePitchChange = (float)deltaY * 0.25f;

    // in SWIMMING MODE FROM (-70;70), in WALKING MODE FROM (0;70) !!!!!!
    
    if(anglePitch + anglePitchChange <= 70 && anglePitch + anglePitchChange >= -40) {
        anglePitch += anglePitchChange;
    }

    angleYawChange = (float)deltaX * 0.25f;
    if(angleYaw > 360) {
        angleYaw -= 360;
    }
    if(angleYaw < 0) {
        angleYaw += 360;
    }
    angleYaw -= angleYawChange;
}

float Camera::horizontalDistance() {
    return distance * cos(glm::radians(anglePitch));
}

float Camera::verticalDistance() {
    return distance * sin(glm::radians(anglePitch));
}

void Camera::calculateCameraPosition(float horizontalD, float verticalD) {
    float angle = angleYaw;
    float offsetX = horizontalD * sin(glm::radians(angle));
    float offsetZ = horizontalD * cos(glm::radians(angle));
    position.x = robot->position.x - offsetX;
    position.z = robot->position.z - offsetZ;
    position.y = robot->position.y + this->offset + verticalD;

    glm::vec3 cameraTransformation = position - (robot->position + glm::vec3(0.0f, 0.5f, 0.0f));
    glm::vec3 invertedDirection = glm::normalize(cameraTransformation);

    glm::vec3 raycastOrigin = (robot->position + glm::vec3(0.0f, 0.5f, 0.0f) + invertedDirection * 0.1f);

    physx::PxVec3 origin = physx::PxVec3(raycastOrigin.x, raycastOrigin.y, raycastOrigin.z);
    physx::PxReal maxDistance = distance;
    physx::PxRaycastBuffer hit;
    physx::PxQueryFilterData filterData(physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC);
    filterData.data.word0 = 2;
    physx::PxVec3 unitDir = physx::PxVec3(invertedDirection.x, invertedDirection.y, invertedDirection.z);
    
    bool status = physicsObject->scene->raycast(origin, unitDir, maxDistance, hit, ((physx::PxHitFlags)(physx::PxHitFlag::eDEFAULT)), filterData);
    if (hit.hasAnyHits()) {
        if (glm::fastDistance(position, cameraTarget) > 1.0f) {
            position = glm::vec3(hit.block.position.x, hit.block.position.y, hit.block.position.z)
            + glm::vec3(hit.block.normal.x, hit.block.normal.y, hit.block.normal.z) * 0.1f;
        }
    } 

    float worldY = worldObject->getHeightAt(position.x, position.z);

    if (position.y > 192.0f && position.y < 192.2f) {
        position.y = 192.2f;
    }

    if (position.y < worldY) {
        position.y = worldY;
    }
}

void Camera::increaseCameraDistance() {
    if( distance < 2.2f ) {
        distance += 0.1f;
        position -= getDirection() * 0.1f;
    }
}

void Camera::decreaseCameraDistance() {
    if( distance > 0.8f ) {
        distance -= 0.1f;
        position += getDirection() * 0.1f;
    }
}

void Camera::setDirection() {
    direction = glm::normalize(position - cameraTarget);
}

glm::vec3 Camera::getPosition() {
    return position;
}

float Camera::getPitch() {
    return anglePitch;
}

glm::vec3 Camera::getDirection() {
    return direction;
}

void Camera::updatePerspective() {
    perspective = glm::perspective(glm::radians(fov), (float) width / (float) height, near, far);
}

void Camera::update() {
    if (controller->mouseRightClicked) {
        this->offset = std::min(this->offset + 0.01f, 1.0f + BASE_CAMERA_OFFSET);
    } else {
        this->offset = std::max(this->offset - 0.01f, BASE_CAMERA_OFFSET);
    }
    cameraTarget = glm::vec3(robot->position) + glm::vec3(0.0f, this->offset, 0.0f);
    
    float horizontalDistance = this->horizontalDistance();
    float verticalDistance = this->verticalDistance();
    calculateCameraPosition(horizontalDistance, verticalDistance);

    setDirection();

    this->rotationMatrix = glm::lookAt(position, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    sound->setPosition(this->position.x, this->position.y, this->position.z);
    float soundOrientation[6];
    soundOrientation[0] = cameraTarget.x - this->position.x;
    soundOrientation[1] = cameraTarget.y - this->position.y;
    soundOrientation[2] = cameraTarget.z - this->position.z;
    soundOrientation[3] = 0.0f;
    soundOrientation[4] = 1.0f;
    soundOrientation[5] = 0.0f;
    sound->setOrientation(soundOrientation);
}

Camera *camera;