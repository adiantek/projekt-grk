#pragma once

class Time {
  public:
    Time();
    ~Time();

    void update();
    
    float deltaTime = 0.0f;
    int fps = 0;

    inline static float time = 0.0f;

  private:
    double lastFrame = 0;
    int frameCount = 0;
};

extern Time *timeExternal;