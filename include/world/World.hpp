#pragma once

#include <Camera/Skybox.hpp>
#include <Robot/Robot.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <deque>
#include <unordered_map>
#include <world/Chunk.hpp>
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
    std::deque<ChunkPosition> chunksQueue;
    std::deque<ChunkPosition> chunksQueueDrop;
    bool shouldChunkBeLoaded(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t viewDistance);
    void loadChunkQueue(ChunkPosition pos);
    void unloadChunkQueue(ChunkPosition pos);
    void loadChunkNow(ChunkPosition pos);
    void unloadChunkNow(ChunkPosition pos);
    void updateChunkMap(bool firstLoad);
    std::unordered_map<uint64_t, Chunk *> chunks;

    double lastLoadChunks = 0;
    void loadChunks();
    void updateChunks();
    void drawChunks(glm::mat4 mat);

   public:
    int64_t seed;
    SimplexNoiseGenerator *noise;

    World(int64_t seed);
    virtual ~World();

    void update() override;
    void draw(glm::mat4 mat) override;
    Chunk *getChunkAt(ChunkPosition pos);
};

}  // namespace world