#pragma once

#include <opengl.h>
#include <stdbool.h>

#include <Camera.hpp>

#ifdef __cplusplus
extern "C" {
#endif

class Controller {
   public:
    Controller(GLFWwindow *window, Camera *camera);
    ~Controller();
    GLFWwindow *window;
    Camera *camera;
    float distance = 1.0f;
    bool keys[GLFW_KEY_LAST];
    bool mouseGrabbed = false;
    bool firstMouseMove = false;

    void window_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void update();

   private:
    double lastX = 0, lastY = 0;
};

extern Controller *controller;
static void s_window_size_callback(GLFWwindow *window, int width, int height);
static void s_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void s_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
static void s_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
static void s_cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE void mouse_grab_status(bool active);
#endif

#ifdef __cplusplus
}
#endif