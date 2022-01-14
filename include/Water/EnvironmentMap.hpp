#pragma once

#include<glm/glm.hpp>
#include<Render_Utils.h>

namespace water {
    class EnvironmentMap {
    public:
        EnvironmentMap(float size, float y, unsigned int textureSize, glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, -1.0f), float maxDepth = 50.0f);
        ~EnvironmentMap();
        void useFramebuffer();
        void stopUsingFramebuffer();
        void drawObject(Core::RenderContext context, glm::mat4 modelMatrix);
        unsigned int getMapTexture();
        glm::mat4 getLightCamera();
    private:
        float size;
        unsigned int textureSize;
        float y;
        glm::vec3 lightDirection;
        glm::mat4 lightCameraProjectionMatrix;
        glm::mat4 lightCameraRotationMatrix;
        glm::vec3 lightCameraTranslation;
        unsigned int framebuffer;
        unsigned int depthbuffer;
        unsigned int texture;
        glm::mat4 lightCameraMatrix;
        int prevViewport[4];
    };
}