#pragma once
#include <Water/Caustics.hpp>
#include <Water/Simulation.hpp>
#include <Water/Surface.hpp>
#include <world/Object3D.hpp>

namespace water {
class Water : world::Object3D {
   public:
    Water(float size, float y, unsigned int texureSize);
    ~Water();
    void draw(glm::mat4 viewMatrix) override;
    void update() override;
    void useFramebuffer();
    void drawObject(Core::RenderContext context, glm::mat4 modelMatrix);
    void stopUsingFramebuffer();
    glm::mat4 getLightCamera();
    unsigned int getCausticsMap();
    void addWorldObject(world::Object3D* object);
    void removeWorldObject(world::Object3D* object);
    void clearWorldObjects();

   private:
    Caustics caustics;
    Simulation simulation[8];
    Surface surface[9];
    double lastUpdateTime = -1.0;
};
}  // namespace water

extern water::Water* waterObject;