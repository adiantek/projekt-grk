#pragma once

#include <opengl.h>
#include <stdbool.h>
#include <stdint.h>

#include <Physics/RigidBody.hpp>
#include <Random.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <world/Chest.hpp>
#include <world/ChunkPosition.hpp>
#include <world/Object3D.hpp>

namespace world {

class World;

class Chunk : Object3D {
   private:
    GLuint vao, vbo, elements;
    GLuint vaoLines, vboLines;
    double created;
    physics::RigidBody *rigidBody;
    bool minFishYCalculated = false;
    int64_t seed;
    float heightMap[17 * 17];
    Random *chunkRandom;
    float *kelps_matrices;
    int32_t kelps_len;
    size_t *kelps;
    size_t *kelps_shadow;
    float *grass_matrices;
    int32_t grass_len;
    size_t *grass;
    size_t *grass_shadow;
    Chest *chest = 0;

   public:
    ChunkPosition pos;
    World *world;
    float maxY;
    float minY;
    float allowFishAbove = 256;
    bool frustumVisible = false;
    bool frustumShadowVisible = false;
    Chunk(World *world, ChunkPosition pos, float *noise);
    virtual ~Chunk();

    /**
     * delete Random after use
     */
    Random *createChunkRandom();
    void generate(float *noise);
    /**
     * @brief step one - decorate only this chunk
     */
    void decorate1();
    /**
     * @brief step two - decorate if neighbours loaded
     */
    void decorate2();

    void onShow();
    void onHide();
    void onShadowShow();
    void onShadowHide();

    /**
     * @brief equals to getHeightAt((float)x, (float)z);
     */
    float getHeightAt(int32_t x, int32_t z);
    float getHeightAt(float x, float z);

    static void prepareRendering(glm::mat4 mat);
    void update() override;
    void drawTerrain(glm::mat4 mat);
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
};
}  // namespace world