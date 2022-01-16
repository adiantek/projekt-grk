#pragma once

#include <world/Object3D.hpp>
#include <world/ChunkBorderDebugRenderer.hpp>

namespace world {

class World : Object3D {
   private:
    ChunkBorderDebugRenderer *chunkBorderDebugRenderer;

   public:
    World();
    virtual ~World();

    void update() override;
    void draw(glm::mat4 mat) override;
};

}  // namespace world