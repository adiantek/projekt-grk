#include <cmath>
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
    this->lastFrame = currentFrame;
    this->lastFrames.push_back(currentFrame);
    while (currentFrame - this->lastFrames.front() >= 1.0) {
        this->lastFrames.pop_front();
    }
    this->fps = (int) this->lastFrames.size();
}

Time *timeExternal;