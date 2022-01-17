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

ChunkPosition::ChunkPosition(glm::vec3 vec) {
    int32_t ix = (int32_t)vec.x;
    int32_t iz = (int32_t)vec.z;
    this->coords.x = (vec.x < (float)ix ? ix - 1 : ix) >> 4;
    this->coords.z = (vec.z < (float)iz ? iz - 1 : iz) >> 4;
}