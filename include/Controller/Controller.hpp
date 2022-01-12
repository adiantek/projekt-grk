#pragma once

#include <list>
#include <functional>

#include <opengl.h>
#include <stdbool.h>
#include <glm/ext.hpp>

class Controller {
   public:
    Controller(GLFWwindow *window);
    ~Controller();

    GLFWwindow *window;
    bool keys[GLFW_KEY_LAST];
    bool mouseGrabbed = false;
    
    void init();
    void update();

    static const int MODE_PASSIVE = 0;
    static const int MODE_ACTIVE = 1;

    static void onWindowSizeChange(GLFWwindow *window, int width, int height);
    static void onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void onMouseButtonPress(GLFWwindow *window, int button, int action, int mode);
    static void onScroll(GLFWwindow *window, double xOffset, double yOffset);
    static void onCursorPositionChange(GLFWwindow *window, double xPos, double yPos);

   private:
    int mode;

    bool firstMouseMove = false;
    double lastX = 0, lastY = 0;
};

extern Controller *controller;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE void mouse_grab_status(bool active);
#endif