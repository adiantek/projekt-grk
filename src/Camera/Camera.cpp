#include <opengl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Camera/Camera.hpp>
#include <Controller/Controller.hpp>

Camera::Camera(int width, int height, float fov, float near, float far, int x, int y) {
    camera = this;

    this->width = width;
    this->height = height;
    this->fov = fov; // fov = field of view
    this->near = near;
    this->far = far;
    this->x = x;
    this->y = y;

    // setRotation(glm::quat());
    updatePerspective();
}

Camera::~Camera() {}

glm::mat4 Camera::getTransformationMatrix() {
    return perspective * glm::mat4_cast(-rotation) * glm::translate(-position);
}

void Camera::useCameraViewport() {
    glViewport(x, y, width, height);
}

void Camera::setSize(int width, int height) {
    this->width = width;
    this->height = height;
    updatePerspective();
}

void Camera::setFov(float fov) {
    this->fov = fov;
    updatePerspective();
}

void Camera::increaseCameraDistance() {
    if( distance < 1.5f ) {
        distance += 0.1f;
        position -= getDirection() * 0.1f;
    }
}

void Camera::decreaseCameraDistance() {
    if( distance > -0.0f ) {
        distance -= 0.1f;
        position += getDirection() * 0.1f;
    }
}

// void Camera::setRotation(glm::quat rotation) {
//     this->rotation = glm::normalize(rotation);
//     glm::quat inverse = glm::inverse(this->rotation);
//     direction = inverse * glm::vec3(0.0f, 0.0f, -1.0f);
//     up = inverse * glm::vec3(0.0f, 1.0f, 0.0f);
//     side = glm::cross(direction, up);
// }

// void Camera::rotate(glm::quat rotation) {
//     setRotation(rotation * this->rotation);
// }

// glm::quat Camera::getRotation() {
//     return rotation;
// }

glm::vec3 Camera::getDirection() {
    return direction;
}

glm::vec3 Camera::getUp() {
    return up;
}

glm::vec3 Camera::getSide() {
    return side;
}

float Camera::getAspect() {
    return (float) width / (float) height;
}

void Camera::updatePerspective() {
    perspective = glm::perspective(glm::radians(fov), (float) width / (float) height, near, far);
}

Camera *camera;