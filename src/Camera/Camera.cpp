#include <opengl.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Camera/Camera.hpp>
#include <Controller/Controller.hpp>
#include <Robot/Robot.hpp>
#include <Time/Time.hpp>
#include <Glow/GlowShader.hpp>

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

void Camera::useCameraViewport() {
    glViewport(x, y, width, height);
}

void Camera::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    updatePerspective();
    Glow::glow->resize(width, height);
}

void Camera::setFov(float fov) {
    this->fov = fov;
    updatePerspective();
}

void Camera::mouseMove(double deltaX, double deltaY) {
    anglePitchChange = (float)deltaY * 0.25f;

    // in SWIMMING MODE FROM (-70;70), in WALKING MODE FROM (0;70) !!!!!!
    
    if(anglePitch + anglePitchChange <= 70 && anglePitch + anglePitchChange >= -70) {
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
    position.y = robot->position.y + verticalD;
}

void Camera::increaseCameraDistance() {
    if( distance < 2.0f ) {
        distance += 0.1f;
        position -= getDirection() * 0.1f;
    }
}

void Camera::decreaseCameraDistance() {
    if( distance > 0.5f ) {
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
    cameraTarget = glm::vec3(robot->position) + glm::vec3(0.0f, 0.2f, 0.0f);
    
    float horizontalDistance = this->horizontalDistance();
    float verticalDistance = this->verticalDistance();
    calculateCameraPosition(horizontalDistance, verticalDistance);

    setDirection();

    this->rotationMatrix = glm::lookAt(position, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

Camera *camera;