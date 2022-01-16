#include <world/World.hpp>

using namespace world;

World::World() {
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
}

World::~World() {
    delete this->chunkBorderDebugRenderer;
    delete this->crosshair;
}

void World::update() {
    this->chunkBorderDebugRenderer->update();
    this->crosshair->update();
}

void World::draw(glm::mat4 mat) {
    this->chunkBorderDebugRenderer->draw(mat);
    this->crosshair->draw(mat);
}