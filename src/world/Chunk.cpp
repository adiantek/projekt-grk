#include <world/Chunk.hpp>
using namespace world;

Chunk::Chunk(int64_t seed, ChunkPosition pos) {
    this->seed = seed;
    this->pos = pos;
}

Chunk::~Chunk() {
}

Random *Chunk::createChunkRandom() {
    Random *r = new Random(this->seed);
    int64_t xa = r->nextLong();
    int64_t xb = r->nextLong();
    r->setSeed((int64_t)this->pos.coords.x * xa ^ (int64_t)this->pos.coords.z * xb ^ seed);
    return r;
}


void Chunk::generate(SimplexNoiseGenerator *noise, ResourceLoader *res) {
    noise->draw(res);
    
}