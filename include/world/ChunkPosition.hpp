#pragma once

#include <stdint.h>

#include <glm/glm.hpp>

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
    ChunkPosition(glm::vec3 vec);
} ChunkPosition;

}  // namespace world
