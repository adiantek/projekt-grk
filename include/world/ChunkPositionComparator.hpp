#pragma once

#include <stdbool.h>

#include <glm/glm.hpp>
#include <world/ChunkPosition.hpp>

namespace world {

extern glm::vec3 ChunkPositionComparator_translate;
bool ChunkPositionComparator_comparator(ChunkPosition a, ChunkPosition b);
bool ChunkPositionComparator_comparatorP(ChunkPosition *a, ChunkPosition *b);

}  // namespace world