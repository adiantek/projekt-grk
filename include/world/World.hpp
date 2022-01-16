#pragma once

#include <world/Object3D.hpp>
#include <world/ChunkBorderDebugRenderer.hpp>
#include <world/Crosshair.hpp>
#include <Camera/Skybox.hpp>
#include <Robot/Robot.hpp>

namespace world {

class World : Object3D {
   private:
    cam::Skybox *skybox;
    ChunkBorderDebugRenderer *chunkBorderDebugRenderer;
    Crosshair *crosshair;
    entity::Robot *robot;

   public:
    World();
    virtual ~World();

    void update() override;
    void draw(glm::mat4 mat) override;
};

}  // namespace world