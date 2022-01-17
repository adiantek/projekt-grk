#include <world/Chunk.hpp>
#include <Logger.h>

using namespace world;

Chunk::Chunk(int64_t seed, ChunkPosition pos) {
    this->seed = seed;
    this->pos = pos;
    LOGD("Chunk: loading %d %d", pos.coords.x, pos.coords.z);
}

Chunk::~Chunk() {
    LOGD("Chunk: unloading %d %d", pos.coords.x, pos.coords.z);
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

void Chunk::update() {
}

void Chunk::draw(glm::mat4 mat) {

}