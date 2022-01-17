#pragma once

#include <Render_Utils.h>

#include <glm/ext.hpp>
#include <world/Object3D.hpp>

namespace water {
class Surface : world::Object3D {
   public:
    Surface(float size, float y, unsigned int textureSize, unsigned int heightMap, unsigned int normalMap, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    ~Surface();
    void draw(glm::mat4 viewMatrix) override;
    void update() override;

   private:
    float size;
    float y;
    unsigned int heightMap;
    unsigned int normalMap;
    glm::vec2 offset;
    Core::RenderContext geometry;
    unsigned int skybox;
    glm::mat4 rotation = glm::eulerAngleX(glm::radians(90.0f));
    glm::vec2 lastCameraPosition;
};
}  // namespace water