#include <world/ChunkPositionComparator.hpp>

using namespace world;

glm::vec2 world::ChunkPositionComparator_translate;

bool world::ChunkPositionComparator_comparator(ChunkPosition a, ChunkPosition b) {
    return ChunkPositionComparator_comparatorP(&a, &b);
}

bool world::ChunkPositionComparator_comparatorP(ChunkPosition *a, ChunkPosition *b) {
    if (a->id == b->id) {
        return 0;
    }
    float ax = a->coords.x - ChunkPositionComparator_translate.x;
    float az = a->coords.z - ChunkPositionComparator_translate.y;
    float bx = b->coords.x - ChunkPositionComparator_translate.x;
    float bz = b->coords.z - ChunkPositionComparator_translate.y;

    // a^2 - b^2 = (a-b)(a+b)
    int result = (int)(((ax - bx) * (ax + bx)) + ((az - bz) * (az + bz)));
    if (result) {
        return result < 0;
    }

    if (ax < 0) {
        if (bx < 0) {
            return (bz - az) < 0;
        } else {
            return (-1) < 0;
        }
    } else {
        if (bx < 0) {
            return (1) < 0;
        } else {
            return (az - bz) < 0;
        }
    }
}