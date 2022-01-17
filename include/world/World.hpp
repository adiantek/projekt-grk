#pragma once

#include <Camera/Skybox.hpp>
#include <Robot/Robot.hpp>
#include <vector>
#include <world/ChunkBorderDebugRenderer.hpp>
#include <world/Crosshair.hpp>
#include <world/Object3D.hpp>

namespace world {

class World : Object3D {
   private:
    cam::Skybox *skybox;
    ChunkBorderDebugRenderer *chunkBorderDebugRenderer;
    Crosshair *crosshair;
    entity::Robot *robot;

    glm::vec2 updateChunkRobotPos;
    ChunkPosition updateChunkLastPos;
    std::vector<ChunkPosition> chunksQueue;
    bool shouldChunkBeLoaded(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t viewDistance);
    void loadChunk(ChunkPosition pos);
    void unloadChunk(ChunkPosition pos);
    void updateChunks(bool firstLoad);
    bool matrix[20 * 20];

   public:
    World();
    virtual ~World();

    void update() override;
    void draw(glm::mat4 mat) override;
};

}  // namespace world