#pragma once
#include<Water/Simulation.hpp>
#include<Water/Caustics.hpp>
#include<Water/Surface.hpp>

namespace water {
    class Water {
    public:
        Water(float size, float y, unsigned int texureSize);
        ~Water();
        void draw();
        void simulate();
        void useFramebuffer();
        void drawObject(Core::RenderContext context, glm::mat4 modelMatrix);
        void stopUsingFramebuffer();
        glm::mat4 getLightCamera();
        unsigned int getCausticsMap();
    private:
        Caustics caustics;
        Simulation simulation[8];
        Surface surface[9];
        float lastTime;
    };
}

extern water::Water* waterObject;