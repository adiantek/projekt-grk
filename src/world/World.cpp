#include <Logger.h>

#include <Time/Time.hpp>
#include <world/ChunkPositionComparator.hpp>
#include <world/World.hpp>

using namespace world;

#define VIEW_DISTANCE 16

World::World(int64_t seed) {
    this->seed = seed;
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
    this->robot = new entity::Robot();
    Random r(seed);
    this->noise = new SimplexNoiseGenerator(&r);
    // this->noise->debugNoise(0, 0);
    // this->noise->debugNoise(0, 1);
    // this->noise->debugNoise(0, 2);

    this->updateChunkMap(true);
}

World::~World() {
    delete this->chunkBorderDebugRenderer;
    delete this->crosshair;
    delete this->skybox;
    delete this->robot;
}

bool World::shouldChunkBeLoaded(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t viewDistance) {
    int32_t x = x1 - x2;
    int32_t y = y1 - y2;

    return x >= -viewDistance && x <= viewDistance && y >= -viewDistance && y <= viewDistance;
}

void World::loadChunkNow(ChunkPosition pos) {
    Chunk *ch = new Chunk(this, pos);
    this->chunks[pos.id] = ch;
}

void World::unloadChunkNow(ChunkPosition pos) {
    delete this->chunks[pos.id];
    this->chunks.erase(pos.id);
}

void World::loadChunkQueue(ChunkPosition pos) {
    for (std::deque<ChunkPosition>::reverse_iterator it = this->chunksQueueDrop.rbegin(); it != this->chunksQueueDrop.rend(); ++it) {
        if ((*it).id == pos.id) {
            if (this->chunksQueueDrop.back().id == pos.id) {
                this->chunksQueueDrop.pop_back();
            } else {
                *it = this->chunksQueueDrop.back();
                this->chunksQueueDrop.pop_back();
            }
            return;
        }
    }
    this->chunksQueue.push_back(pos);
}

void World::unloadChunkQueue(ChunkPosition pos) {
    for (std::deque<ChunkPosition>::reverse_iterator it = this->chunksQueue.rbegin(); it != this->chunksQueue.rend(); ++it) {
        if ((*it).id == pos.id) {
            if (this->chunksQueue.back().id == pos.id) {
                this->chunksQueue.pop_back();
            } else {
                *it = this->chunksQueue.back();
                this->chunksQueue.pop_back();
            }
            return;
        }
    }
    this->chunksQueueDrop.push_back(pos);
}

void World::updateChunkMap(bool firstLoad) {
    glm::vec3 pos = this->robot->position;
    if (!firstLoad) {
        glm::vec2 pos2d = glm::vec2(pos.x, pos.z);
        float dist = glm::distance2(pos2d, this->updateChunkRobotPos);
        if (dist < 8 * 8) {
            return;
        }
        this->updateChunkRobotPos = pos2d;
    }
    ChunkPosition curr(pos);
    LOGI("Updating chunks");

    int32_t currX = curr.coords.x;
    int32_t currZ = curr.coords.z;

    int32_t prevX = this->updateChunkLastPos.coords.x;
    int32_t prevZ = this->updateChunkLastPos.coords.z;

    this->updateChunkLastPos = curr;

    int32_t viewDistance = VIEW_DISTANCE;

    int32_t diffX = currX - prevX;
    int32_t diffZ = currZ - prevZ;

    if (diffX == 0 && diffZ == 0 && !firstLoad) {
        return;
    }
    for (int32_t x = currX - viewDistance; x <= currX + viewDistance; x++) {
        for (int32_t z = currZ - viewDistance; z <= currZ + viewDistance; z++) {
            if (firstLoad || !this->shouldChunkBeLoaded(x, z, prevX, prevZ, viewDistance)) {
                this->loadChunkQueue(ChunkPosition(x, z));
            }
            if (!this->shouldChunkBeLoaded(x - diffX, z - diffZ, currX, currZ, viewDistance)) {
                this->unloadChunkQueue(ChunkPosition(x - diffX, z - diffZ));
            }
        }
    }
    ChunkPositionComparator_translate = glm::vec2(pos.x, pos.z) / 16.0F;
    std::sort(this->chunksQueue.begin(), this->chunksQueue.end(), ChunkPositionComparator_comparator);
    std::sort(this->chunksQueueDrop.begin(), this->chunksQueueDrop.end(), ChunkPositionComparator_comparator);
}

void World::loadChunks() {
    if (timeExternal->lastFrame - this->lastLoadChunks < 0.01) {  // 100 chunks per sec
        return;
    }
    this->lastLoadChunks = timeExternal->lastFrame;
    if (this->chunksQueue.size() > 0) {
        this->loadChunkNow(this->chunksQueue.front());
        this->chunksQueue.pop_front();
    }
    if (this->chunksQueueDrop.size() > 0) {
        this->unloadChunkNow(this->chunksQueueDrop.back());
        this->chunksQueueDrop.pop_back();
    }
}

void World::updateChunks() {
    for (auto &it : this->chunks) {
        Chunk *ch = it.second;
        ch->update();
    }
}

void World::drawChunks(glm::mat4 mat) {
    for (auto &it : this->chunks) {
        Chunk *ch = it.second;
        ch->draw(mat);
    }
}

void World::drawShadowChunks(glm::mat4 mat) {
    for (auto &it : this->chunks) {
        Chunk *ch = it.second;
        ch->drawShadow(mat);
    }
}

void World::update() {
    this->chunkBorderDebugRenderer->update();
    this->crosshair->update();
    this->skybox->update();
    // this->robot->update();
    this->updateChunkMap(false);
    this->loadChunks();
    this->updateChunks();
}

void World::draw(glm::mat4 mat) {
    glEnable(GL_CULL_FACE);

    this->skybox->draw(mat);  // na poczatku - depth offniety

    // this->chunkBorderDebugRenderer->draw(mat);
    this->drawChunks(mat);
    this->robot->draw(mat);

    this->crosshair->draw(mat);  // na koncu - depth offniety

    glDisable(GL_CULL_FACE);
}

void World::drawShadow(glm::mat4 mat) {
    this->drawShadowChunks(mat);
    this->robot->drawShadow(mat);
}