#pragma once

#include <world/Object3D.hpp>
#include <world/ChunkBorderDebugRenderer.hpp>
#include <world/Crosshair.hpp>

namespace world {

class World : Object3D {
   private:
    ChunkBorderDebugRenderer *chunkBorderDebugRenderer;
    Crosshair *crosshair;

   public:
    World();
    virtual ~World();

    void update() override;
    void draw(glm::mat4 mat) override;
};

}  // namespace world