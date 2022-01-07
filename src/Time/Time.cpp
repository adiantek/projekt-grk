#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Time/Time.hpp>

Time::Time() {
    timeExternal = this;
}

Time::~Time() {}

void Time::update() {
    double currentFrame = glfwGetTime();
    this->deltaTime = (float)(currentFrame - lastFrame);

    frameCount++;

    if (currentFrame - lastFrame >= 1.0) {
        this->fps = frameCount;
        frameCount = 0;
    }

    this->lastFrame = currentFrame;
}

Time *timeExternal;