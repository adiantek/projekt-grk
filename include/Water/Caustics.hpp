#pragma once

#include<Render_Utils.h>
#include<Water/EnvironmentMap.hpp>

namespace water {
    class Caustics {
    public:
        Caustics(float size, unsigned int textureSize, float y, unsigned int heightMap, unsigned int normalMap);
        ~Caustics();
        void render();
        unsigned int getTexture();

        glm::mat4 getLightCamera();
        
        EnvironmentMap environmentMap;
    private:
        float size;
        unsigned int textureSize;
        float y;
        unsigned int heightMap;
        unsigned int normalMap;
        Core::RenderContext geometry;
        unsigned int framebuffer;
        unsigned int depthbuffer;
        unsigned int texture;
    };
}