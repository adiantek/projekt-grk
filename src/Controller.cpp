#include <Logger.h>

#include <Controller.hpp>

Controller::Controller(GLFWwindow *window, Camera *camera) {
    controller = this;
    for (int i = 0; i < GLFW_KEY_LAST; i++) {
        this->keys[i] = false;
    }
    this->window = window;
    this->camera = camera;
    glfwSetCursorPosCallback(window, s_cursor_position_callback);
    glfwSetWindowSizeCallback(window, s_window_size_callback);
    glfwSetMouseButtonCallback(window, s_mouse_button_callback);
    glfwSetKeyCallback(window, s_key_callback);
    glfwSetScrollCallback(window, s_scroll_callback);
}

Controller::~Controller() {}

void Controller::window_size_callback(GLFWwindow *window, int width, int height) {
    LOGI("Widow resize: %d x %d", width, height);
    if (width < 1) {
        width = 1;
    }
    if (height < 1) {
        height = 1;
    }
    this->camera->setSize(width, height);
    this->camera->useCameraViewport();
}

void Controller::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key >= GLFW_KEY_LAST) {
        return;
    }
    if (action == 1) {
        LOGI("Pressed key: %d", key);
        this->keys[key] = true;
    } else if (action == 0) {
        LOGI("Released key: %d", key);
        this->keys[key] = false;
    }
}

void Controller::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    LOGI("mouse button: %d, action: %d, mods: %d", button, action, mods);
    if (button == 0 && action == 1) {
        // press left button
#ifndef __EMSCRIPTEN__
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        this->mouseGrabbed = true;
        this->firstMouseMove = true;
#endif
    }
}

void Controller::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (yoffset < 0.0) {
        distance += 0.1f;
        this->camera->position -= this->camera->getDirection() * 0.1f;
    } else {
        distance -= 0.1f;
        this->camera->position += this->camera->getDirection() * 0.1f;
    }
}

void Controller::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    if (!this->mouseGrabbed) {
        return;
    }
    if (this->firstMouseMove) {
        this->lastX = xpos;
        this->lastY = ypos;
        this->firstMouseMove = false;
        return;
    }
    double deltaX = xpos - this->lastX;
    double deltaY = ypos - this->lastY;
    this->lastX = xpos;
    this->lastY = ypos;
    if (mouseGrabbed) {
        glm::quat mx = glm::angleAxis((float)deltaX * 0.005f, glm::vec3(0.0, 1.0, 0.0));
        glm::quat my = glm::angleAxis((float)deltaY * 0.005f, glm::vec3(1.0, 0.0, 0.0));

        this->camera->rotate(mx * my);
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
    float angleSpeed = 0.01f;
    float moveSpeed = 0.05f;
    if (this->keys[GLFW_KEY_Z]) {
        this->camera->rotate(
            glm::angleAxis(-angleSpeed, glm::vec3(0.0, 0.0, 1.0)));
    }
    if (this->keys[GLFW_KEY_X]) {
        this->camera->rotate(
            glm::angleAxis(angleSpeed, glm::vec3(0.0, 0.0, 1.0)));
    }
    if (this->keys[GLFW_KEY_W]) {
        this->camera->position += this->camera->getDirection() * moveSpeed;
    }
    if (this->keys[GLFW_KEY_S]) {
        this->camera->position -= this->camera->getDirection() * moveSpeed;
    }
    if (this->keys[GLFW_KEY_D]) {
        this->camera->position += this->camera->getSide() * moveSpeed;
    }
    if (this->keys[GLFW_KEY_A]) {
        this->camera->position -= this->camera->getSide() * moveSpeed;
    }
    if (this->keys[GLFW_KEY_LEFT_SHIFT]) {
        this->camera->position -= this->camera->getUp() * moveSpeed;
    }
    if (this->keys[GLFW_KEY_SPACE]) {
        this->camera->position += this->camera->getUp() * moveSpeed;
    }
}

static void s_window_size_callback(GLFWwindow *window, int width, int height) {
    controller->window_size_callback(window, width, height);
}

static void s_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    controller->key_callback(window, key, scancode, action, mods);
}

static void s_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    controller->mouse_button_callback(window, button, action, mods);
}

static void s_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    controller->scroll_callback(window, xoffset, yoffset);
}

static void s_cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    controller->cursor_position_callback(window, xpos, ypos);
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
