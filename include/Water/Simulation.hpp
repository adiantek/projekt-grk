#pragma once

#include<Render_Utils.h>
#include<ResourceLoader.hpp>

namespace water {
    class Simulation {
    public:
        Simulation(unsigned int width, unsigned int height, float scale, ResourceLoader *loader);
        ~Simulation();
        void simulate(glm::vec3 transition);
        void generateRandomWaves();
        unsigned int maps[2];
    private:
        unsigned int width;
        unsigned int height;
        float scale;
        unsigned int framebuffer;
        Core::RenderContext geometry;
        unsigned int program;
        int uniformTransition;
        int uniformScale;
        int uniformTime;
        int uniformWaveCount;
    };
}