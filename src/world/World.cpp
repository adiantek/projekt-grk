#include <world/World.hpp>

using namespace world;

World::World() {
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
    this->robot = new entity::Robot();
}

World::~World() {
    delete this->chunkBorderDebugRenderer;
    delete this->crosshair;
    delete this->skybox;
    delete this->robot;
}

void World::update() {
    this->chunkBorderDebugRenderer->update();
    this->crosshair->update();
    this->skybox->update();
    this->robot->update();
}

void World::draw(glm::mat4 mat) {
    this->skybox->draw(mat); // na poczatku - depth offniety
    
    this->chunkBorderDebugRenderer->draw(mat);
    this->robot->draw(mat);

    this->crosshair->draw(mat); // na koncu - depth offniety
}