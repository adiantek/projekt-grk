#pragma once

#include <stdint.h>

#include <Random.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <world/ChunkPosition.hpp>

namespace world {
class Chunk {
   public:
    ChunkPosition pos;
    int64_t seed;
    Chunk(int64_t seed, ChunkPosition pos);
    ~Chunk();

    /**
     * delete Random after use
     */
    Random *createChunkRandom();
    void generate(SimplexNoiseGenerator *noise, ResourceLoader *res);
};
}  // namespace world