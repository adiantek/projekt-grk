#include <Logger.h>

#include <world/ChunkPositionComparator.hpp>
#include <world/World.hpp>

using namespace world;

World::World() {
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
    this->robot = new entity::Robot();

    LOGD("Test sortowania chunkow:");
    ChunkPositionComparator_translate = glm::vec3(0);
    ChunkPosition chunks[] = {
        ChunkPosition(0, 400),
        ChunkPosition(400, 0),
        ChunkPosition(0, -400),
        ChunkPosition(-400, 0),
        ChunkPosition(300, 300),
        ChunkPosition(150, 0),
        ChunkPosition(0, 200),
        ChunkPosition(250, 0)};
    std::sort(chunks, chunks + 7, ChunkPositionComparator_comparator);
    for (int i = 0; i < 7; i++) {
        LOGD("chunk: %d %d", chunks[i].coords.x, chunks[i].coords.z);
    }
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
    this->skybox->draw(mat);  // na poczatku - depth offniety

    this->chunkBorderDebugRenderer->draw(mat);
    this->robot->draw(mat);

    this->crosshair->draw(mat);  // na koncu - depth offniety
}