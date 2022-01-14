#pragma once

#include<Render_Utils.h>
#include<Water/EnvironmentMap.hpp>
#include<Water/Simulation.hpp>

namespace water {
    class Caustics {
    public:
        Caustics(float size, float y, unsigned int textureSize);
        ~Caustics();
        void render();
        unsigned int getCausticsMap();
        unsigned int getHeightMap();
        unsigned int getNormalMap();
        glm::mat4 getLightCamera();
        // TODO: use smotehing like list of object to draw instead of next three methods
        void useFramebuffer();
        void stopUsingFramebuffer();
        void drawObject(Core::RenderContext context, glm::mat4 modelMatrix);
    private:
        EnvironmentMap environmentMap;
        Simulation simulation;
        float size;
        float y;
        unsigned int textureSize;
        Core::RenderContext geometry;
        unsigned int framebuffer;
        unsigned int depthbuffer;
        unsigned int texture;
    };
}