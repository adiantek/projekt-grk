#pragma once

#include<ResourceLoader.hpp>
#include<glm/glm.hpp>
#include<Render_Utils.h>

namespace Water {
    class EnvironmentMap {
    public:
        EnvironmentMap(float size, int textureSize, ResourceLoader* loader);
        ~EnvironmentMap();
        void useFramebuffer(glm::vec3 transition);
        void stopUsingFramebuffer();
        void drawObject(Core::RenderContext context, glm::mat4 modelMatrix);
        unsigned int texture;
    private:
        float size;
        int textureSize;
        unsigned int program;
        int uniformModel;
        int uniformTransformation;
        unsigned int framebuffer;
        unsigned int depthbuffer;
        int prevViewport[4];
        glm::mat4 projectionMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 transformationMatrix;
    };
}