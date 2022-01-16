#include <world/World.hpp>

using namespace world;

World::World() {
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
}

World::~World() {
    delete this->chunkBorderDebugRenderer;
}

void World::update() {
    this->chunkBorderDebugRenderer->update();
}

void World::draw(glm::mat4 mat) {
    this->chunkBorderDebugRenderer->draw(mat);
}