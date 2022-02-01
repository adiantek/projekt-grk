#pragma once

#include <opengl.h>
#include <stdbool.h>
#include <stdint.h>

#include <Random.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <world/ChunkPosition.hpp>
#include <world/Object3D.hpp>
#include <Physics/RigidBody.hpp>

namespace world {

class World;

class Chunk : Object3D {
   private:
    GLuint vao, vbo, elements;
    GLuint vaoLines, vboLines;
    double created;
    physics::RigidBody* rigidBody;
    bool minFishYCalculated = false;

   public:
    ChunkPosition pos;
    int64_t seed;
    World *world;
    float heightMap[17 * 17];
    float maxY;
    float allowFishAbove = 256;
    Chunk(World *world, ChunkPosition pos);
    virtual ~Chunk();

    /**
     * delete Random after use
     */
    Random *createChunkRandom();
    void generate();

    static void setupTextures();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
};
}  // namespace world