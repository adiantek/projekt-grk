#pragma once

#include <Render_Utils.h>

#include <Water/EnvironmentMap.hpp>
#include <Water/Simulation.hpp>

namespace water {
class Caustics {
   public:
    Caustics(float size, float y, unsigned int textureSize, float maxDepth);
    ~Caustics();
    void update();
    unsigned int getCausticsMap();
    glm::mat4 getLightCamera();
    void addWorldObject(world::Object3D* object);
    void removeWorldObject(world::Object3D* object);
    void clearWorldObjects();

   private:
    EnvironmentMap environmentMap;
    Simulation simulation;
    float size;
    float y;
    unsigned int textureSize;
    unsigned int geometrySize;
    Core::RenderContext geometry;
    unsigned int framebuffer;
    unsigned int texture;
};
}  // namespace water