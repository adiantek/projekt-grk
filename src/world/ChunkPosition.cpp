#include <world/ChunkPosition.hpp>

using namespace world;

ChunkPosition::ChunkPosition() {
    this->id = 0;
}

ChunkPosition::ChunkPosition(int32_t x, int32_t z) {
    this->coords.x = x;
    this->coords.z = z;
}

ChunkPosition::ChunkPosition(uint64_t id) {
    this->id = id;
}
