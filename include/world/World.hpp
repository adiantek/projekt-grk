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
#include <utils/MatrixTextureArray.hpp>
#include <utils/Frustum.hpp>

namespace world {

class World : Object3D {
   private:
    cam::Skybox *skybox;
    ChunkBorderDebugRenderer *chunkBorderDebugRenderer;
    Crosshair *crosshair;
    entity::Robot *robot;

    glm::vec2 updateChunkRobotPos;
    ChunkPosition updateChunkLastPos;
    ChunkPosition noiseValues;
    std::deque<ChunkPosition> chunksQueue;
    std::deque<ChunkPosition> chunksQueueDrop;
    bool shouldChunkBeLoaded(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t viewDistance);
    void loadChunkQueue(ChunkPosition pos);
    void unloadChunkQueue(ChunkPosition pos);
    void loadChunkNow(ChunkPosition pos, float *noise);
    void unloadChunkNow(ChunkPosition pos);
    void updateChunkMap(bool firstLoad);
    std::unordered_map<uint64_t, Chunk *> chunks;

    double lastLoadChunks = 0;
    void loadChunks();
    void updateChunks();
    void drawChunks(glm::mat4 mat);
    void drawShadowChunks(glm::mat4 mat);
   public:
    int32_t frustumTotal, frustumDraw, frustumDrawDecorator;

    int64_t seed;
    SimplexNoiseGenerator *noise;
    utils::Frustum *frustum;
    void (*onChunkLoad)(Random *random, world::ChunkPosition pos);

    World(int64_t seed, void (*onChunkLoad)(Random *random, world::ChunkPosition pos));
    virtual ~World();

    /**
     * @brief equals to getHeightAt((float)x, (float)z);
     */
    float getHeightAt(int32_t x, int32_t z);
    float getHeightAt(float x, float z);

    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    Chunk *getChunkAt(ChunkPosition pos);
    bool chunksLoaded(glm::vec3 pos);

    void toggleChest();

    utils::MatrixTextureArray kelp;
    utils::MatrixTextureArray seagrass;
    utils::MatrixTextureArray kelpShadow;
    utils::MatrixTextureArray seagrassShadow;
};

}  // namespace world

extern world::World *worldObject;