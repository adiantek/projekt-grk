#pragma once
#include <Water/Caustics.hpp>
#include <Water/Simulation.hpp>
#include <Water/Surface.hpp>
#include <world/Object3D.hpp>

namespace water {
class Water : world::Object3D {
   public:
    Water(float y, float maxDepth, float causticsSize, unsigned int causticsTexureSize, float surfaceSize, unsigned int surfaceTexureSize);
    ~Water();
    void draw(glm::mat4 viewMatrix) override;
    void update() override;
    glm::mat4 getLightCamera();
    unsigned int getCausticsMap();
    void addWorldObject(world::Object3D* object);
    void removeWorldObject(world::Object3D* object);
    void clearWorldObjects();
    float getY();

   private:
    Caustics caustics;
    Surface surface;
    double lastUpdateTime = -1.0;
    float y;
};
}  // namespace water

extern water::Water* waterObject;