#pragma once

#include<Render_Utils.h>

namespace water {
    class Simulation {
    public:
        Simulation(float size, unsigned int textureSize);
        ~Simulation();
        void simulate();
        void generateRandomWaves(long long seed = 2137L);
        unsigned int getHeightMap();
        unsigned int getNormalMap();
    private:
        float size;
        unsigned int textureSize;
        Core::RenderContext geometry;
        unsigned int framebuffer;
        unsigned int maps[2];
    };
}