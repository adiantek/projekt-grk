#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Time {
  public:
    Time();
    ~Time();

    void update();
    
    float deltaTime = 0.0f;
    int fps = 0;

  private:
    double lastFrame = 0;
    int frameCount = 0;
};

extern Time *timeExternal;