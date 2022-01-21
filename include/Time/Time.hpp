#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <deque>

class Time {
   public:
    Time();
    ~Time();

    void update();

    double lastFrame = 0;
    float deltaTime = 0.0f;
    int fps = 0;

   private:
    std::deque<double> lastFrames;
    int frameCount = 0;
};

extern Time *timeExternal;