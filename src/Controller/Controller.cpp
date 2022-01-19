#include <list>
#include <string>
#include <functional>

#include <Logger.h>
#include <Controller/Controller.hpp>
#include <Camera/Camera.hpp>
#include <Robot/Robot.hpp>
#include <ResourceLoader.hpp>

Controller::Controller(GLFWwindow *window) {
    controller = this;

    this->init();
    this->window = window;

    glfwSetWindowSizeCallback(window, onWindowSizeChange);
    glfwSetKeyCallback(window, onKeyPress);
    glfwSetMouseButtonCallback(window, onMouseButtonPress);
    glfwSetScrollCallback(window, onScroll);
    glfwSetCursorPosCallback(window, onCursorPositionChange);
}

Controller::~Controller() {}

void Controller::init() {
    for (int i = 0; i < GLFW_KEY_LAST; i++) {
        this->keys[i] = false;
    }
}

void Controller::onWindowSizeChange(GLFWwindow *window, int width, int height) {
    LOGI("Window resize: %d x %d", width, height);
    if (width < 1) {
        width = 1;
    }
    if (height < 1) {
        height = 1;
    }

    camera->setSize(width, height);
    camera->useCameraViewport();
}

void Controller::onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key >= GLFW_KEY_LAST) {
        return;
    }
    if (action == 1) {
        LOGI("Pressed key: %d", key);
        controller->keys[key] = true;
        if (key == GLFW_KEY_1) {
            glUseProgram(resourceLoaderExternal->p_chunk);
            glUniform1i(resourceLoaderExternal->p_chunk_uni_modeSwitch, 0);
            glUseProgram(0);
        }
        if (key == GLFW_KEY_2) {
            glUseProgram(resourceLoaderExternal->p_chunk);
            glUniform1i(resourceLoaderExternal->p_chunk_uni_modeSwitch, 1);
            glUseProgram(0);
        }
    } else if (action == 0) {
        LOGI("Released key: %d", key);
        controller->keys[key] = false;
    }
}

void Controller::onMouseButtonPress(GLFWwindow *window, int button, int action, int mode) {
    LOGI("mouse button: %d, action: %d", button, action);
    if (button == 0 && action == 1) {
        // press left button
#ifndef __EMSCRIPTEN__
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        controller->mouseGrabbed = true;
        controller->firstMouseMove = true;
#endif
    }
}

void Controller::onScroll(GLFWwindow *window, double xOffset, double yOffset) {
    if (yOffset < 0.0) {
        camera->increaseCameraDistance();
    } else {
        camera->decreaseCameraDistance();
    }
}

void Controller::onCursorPositionChange(GLFWwindow *window, double xpos, double ypos) {
    if (!controller->mouseGrabbed) {
        return;
    }
    if (controller->firstMouseMove) {
        controller->lastX = xpos;
        controller->lastY = ypos;
        controller->firstMouseMove = false;
        return;
    }
    double deltaX = xpos - controller->lastX;
    double deltaY = ypos - controller->lastY;
    controller->lastX = xpos;
    controller->lastY = ypos;
    if (controller->mouseGrabbed) {
        
        camera->mouseMove(deltaX, deltaY);
        
        // glm::quat mx = glm::angleAxis((float)deltaX * 0.005f, glm::vec3(0.0, 1.0, 0.0));
        // glm::quat my = glm::angleAxis((float)deltaY * 0.005f, glm::vec3(1.0, 0.0, 0.0));

        // this->camera->rotate(mx * my);
    }

}

void Controller::update() {
    if (this->keys[GLFW_KEY_ESCAPE]) {
        this->keys[GLFW_KEY_ESCAPE] = 0;
#ifndef __EMSCRIPTEN__
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        this->mouseGrabbed = false;
#endif
    }
    
    if (!this->mouseGrabbed) {
        return;
    }

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::vec3(0.0f, 0.0f, 0.0f);

    camera_direction = camera->getDirection();

    if (this->keys[GLFW_KEY_W]) {
        // position += glm::vec3(0.0f, 0.0f, -1.0f);
        position += glm::vec3(-camera_direction.x, 0.0f, -camera_direction.z);
    }
    if (this->keys[GLFW_KEY_A]) {
        // position += glm::vec3(-1.0f, 0.0f, 0.0f);
        position += glm::cross(glm::vec3(camera_direction.x, 0.0f, camera_direction.z), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (this->keys[GLFW_KEY_S]) {
        // position += glm::vec3(0.0f, 0.0f, 1.0f);
        position += glm::vec3(camera_direction.x, 0.0f, camera_direction.z);
    }
    if (this->keys[GLFW_KEY_D]) {
        // position += glm::vec3(1.0f, 0.0f, 0.0f);
        position += glm::cross(glm::vec3(-camera_direction.x, 0.0f, -camera_direction.z), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (this->keys[GLFW_KEY_SPACE]) {
        position += glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (this->keys[GLFW_KEY_LEFT_ALT]) {
        position += glm::vec3(0.0f, -1.0f, 0.0f);
    }
    if (this->keys[GLFW_KEY_LEFT_SHIFT]) {
        robot->enableIncreasedSpeedMode();
    } else {
        robot->disableIncreasedSpeedMode();
    }
    robot->setMoveDirectionVector(glm::vec3(position));
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE void mouse_grab_status(bool active) {
    LOGI("mouse_grab_status: %s", (active ? "active" : "inactive"));
    controller->mouseGrabbed = active;
    if (active) {
        controller->firstMouseMove = true;
    }
}
#endif

Controller *controller;