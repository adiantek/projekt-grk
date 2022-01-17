#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

class Time {
   public:
    Time();
    ~Time();

    void update();

    float deltaTime = 0.0f;
    int fps = 0;
    double lastFrame = 0;

  private:
    int frameCount = 0;
};

extern Time *timeExternal;