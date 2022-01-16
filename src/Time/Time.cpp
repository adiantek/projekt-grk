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
    this->fps = (int)floor(1.0f / this->deltaTime);
    this->lastFrame = currentFrame;

    Time::deltaTime = this->deltaTime;
    Time::fps = this->fps;
    Time::time = (float)currentFrame;
}

Time *timeExternal;