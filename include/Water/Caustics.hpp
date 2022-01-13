#pragma once

#include<Render_Utils.h>
#include<ResourceLoader.hpp>
#include<Water/EnvironmentMap.hpp>

namespace Water {
    class Caustics {
    public:
        Caustics(float size, int textureSize, unsigned int heightMap, unsigned int normalMap, ResourceLoader *loader);
        ~Caustics();
        glm::mat4 getLightCamera();
        void render();
        unsigned int texture;
        EnvironmentMap environmentMap;
        unsigned int normalMap;
        unsigned int depthMap;
    private:
        float size;
        Core::RenderContext geometry;
        unsigned int program;
        unsigned int textureSize;
        int uniformDeltaEnvTexture;
        int uniformDepthMap;
        int uniformEnvironmentMap;
        int uniformLight;
        int uniformNormalMap;
        int uniformTransformation;
        unsigned int framebuffer;
        unsigned int depthbuffer;
    };
}