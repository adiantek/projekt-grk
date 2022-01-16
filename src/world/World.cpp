#include <world/World.hpp>

using namespace world;

World::World() {
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
}

World::~World() {
    delete this->chunkBorderDebugRenderer;
    delete this->crosshair;
    delete this->skybox;
}

void World::update() {
    this->chunkBorderDebugRenderer->update();
    this->crosshair->update();
    this->skybox->update();
}

void World::draw(glm::mat4 mat) {
    this->skybox->draw(mat);
    this->chunkBorderDebugRenderer->draw(mat);
    this->crosshair->draw(mat);
}