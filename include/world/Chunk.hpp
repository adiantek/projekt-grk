#pragma once

#include <stdint.h>

#include <Random.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <world/ChunkPosition.hpp>
#include <world/Object3D.hpp>

namespace world {
class Chunk : Object3D {
   public:
    ChunkPosition pos;
    int64_t seed;
    Chunk(int64_t seed, ChunkPosition pos);
    virtual ~Chunk();

    /**
     * delete Random after use
     */
    Random *createChunkRandom();
    void generate(SimplexNoiseGenerator *noise, ResourceLoader *res);

    void update() override;
    void draw(glm::mat4 mat) override;
};
}  // namespace world