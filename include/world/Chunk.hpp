#pragma once

#include <opengl.h>
#include <stdbool.h>
#include <stdint.h>

#include <Random.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <world/ChunkPosition.hpp>
#include <world/Object3D.hpp>

namespace world {

class World;

class Chunk : Object3D {
   private:
    GLuint vao, vbo, elements;
    double created;

   public:
    ChunkPosition pos;
    int64_t seed;
    World *world;
    float heightMap[17 * 17];
    Chunk(World *world, ChunkPosition pos);
    virtual ~Chunk();

    /**
     * delete Random after use
     */
    Random *createChunkRandom();
    void generate();

    void update() override;
    void draw(glm::mat4 mat) override;
};
}  // namespace world