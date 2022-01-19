#pragma once

#include <Render_Utils.h>

#include <glm/glm.hpp>
#include <vector>
#include <world/Object3D.hpp>

namespace water {
class EnvironmentMap {
   public:
    EnvironmentMap(float size, float y, unsigned int textureSize, float maxDepth, glm::vec3 lightDirection = glm::vec3(0.0f, 1.0f, 0.0f));
    ~EnvironmentMap();
    void useFramebuffer();
    void stopUsingFramebuffer();
    void drawObject(Core::RenderContext context, glm::mat4 modelMatrix);
    unsigned int getMapTexture();
    glm::mat4 getLightCamera();
    glm::vec3 getLightDirection();
    void setLightDirection(glm::vec3 newLightDirection);
    void addWorldObject(world::Object3D* object);
    void removeWorldObject(world::Object3D* object);
    void clearWorldObjects();
    void update();

   private:
    Core::RenderContext geometry;
    float maxDepth;
    float size;
    unsigned int textureSize;
    float y;
    glm::vec3 lightDirection;
    glm::mat4 lightCameraProjectionMatrix;
    glm::mat4 lightCameraRotationMatrix;
    glm::vec2 lightCameraTranslation;
    std::vector<world::Object3D*> worldObjects;
    unsigned int framebuffer;
    unsigned int depthbuffer;
    unsigned int texture;
    glm::mat4 lightCameraMatrix;
    int prevViewport[4];
};
}  // namespace water