#pragma once

#include <Render_Utils.h>

namespace water {
class Simulation {
   public:
    Simulation(float size, unsigned int textureSize, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    ~Simulation();
    void update();
    void generateRandomWaves(long long seed = 2137L);
    unsigned int getHeightMap();
    unsigned int getNormalMap();

   private:
    float size;
    unsigned int textureSize;
    glm::vec2 offset;
    Core::RenderContext geometry;
    unsigned int framebuffer;
    unsigned int maps[2];
};
}  // namespace water