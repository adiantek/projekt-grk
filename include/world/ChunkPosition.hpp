#pragma once

#include <stdint.h>

namespace world {

typedef struct ChunkCoords {
    int32_t x;
    int32_t z;
} ChunkCoords;

typedef union ChunkPosition {
    uint64_t id;
    ChunkCoords coords;
    ChunkPosition();
    ChunkPosition(int32_t x, int32_t z);
    ChunkPosition(uint64_t id);
} ChunkPosition;

}  // namespace world
