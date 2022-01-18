#include <Logger.h>

#include <Time/Time.hpp>
#include <world/ChunkPositionComparator.hpp>
#include <world/World.hpp>

using namespace world;

#define VIEW_DISTANCE 2

World::World(int64_t seed) {
    this->seed = seed;
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
    this->robot = new entity::Robot();
    Random r(seed);
    this->noise = new SimplexNoiseGenerator(&r);
    this->noise->draw(-123, -123);

    for (int i = 0; i < 20 * 20; i++) {
        this->matrix[i] = false;
    }

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
    Chunk *ch = new Chunk(this->seed, pos);
    this->chunks[pos.id] = ch;
}

void World::unloadChunkNow(ChunkPosition pos) {
    delete this->chunks[pos.id];
    this->chunks.erase(pos.id);
}

void World::loadChunkQueue(ChunkPosition pos) {
    int32_t x = pos.coords.x;
    int32_t z = pos.coords.z;
    int32_t rx = x + 10;
    int32_t rz = z + 10;
    if (rx >= 0 && rx <= 19 && rz >= 0 && rz <= 19) {
        int32_t ci = rx * 20 + rz;
        this->matrix[ci] = true;
    }
    this->chunksQueue.push_back(pos);
    for (std::deque<ChunkPosition>::reverse_iterator it = this->chunksQueueDrop.rbegin(); it != this->chunksQueueDrop.rend(); ++it) {
        if ((*it).id == pos.id) {
            if (this->chunksQueueDrop.back().id == pos.id) {
                this->chunksQueueDrop.pop_back();
            } else {
                *it = this->chunksQueueDrop.back();
                this->chunksQueueDrop.pop_back();
            }
            break;
        }
    }
}

void World::unloadChunkQueue(ChunkPosition pos) {
    int32_t x = pos.coords.x;
    int32_t z = pos.coords.z;
    int32_t rx = x + 10;
    int32_t rz = z + 10;
    if (rx >= 0 && rx <= 19 && rz >= 0 && rz <= 19) {
        int32_t ci = rx * 20 + rz;
        this->matrix[ci] = false;
    }
    this->chunksQueueDrop.push_back(pos);
    for (std::deque<ChunkPosition>::reverse_iterator it = this->chunksQueue.rbegin(); it != this->chunksQueue.rend(); ++it) {
        if ((*it).id == pos.id) {
            if (this->chunksQueue.back().id == pos.id) {
                this->chunksQueue.pop_back();
            } else {
                *it = this->chunksQueue.back();
                this->chunksQueue.pop_back();
            }
            break;
        }
    }
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
    for (int32_t x = 0; x < 20; x++) {
        for (int32_t z = 0; z < 20; z++) {
            printf("%s", this->matrix[x * 20 + z] ? "#" : x == 10 && z == 10 ? "+"
                                                      : x == 10              ? "-"
                                                      : z == 10              ? "|"
                                                                             : " ");
        }
        printf("\n");
    }
}

void World::loadChunks() {
    if (timeExternal->lastFrame - this->lastLoadChunks < 0.05) {  // 20 chunks per sec
        return;
    }
    this->lastLoadChunks = timeExternal->lastFrame;
    if (this->chunksQueue.size() > 0) {
        this->loadChunkNow(this->chunksQueue.front());
        this->chunksQueue.pop_front();
        return;
    }
    if (this->chunksQueueDrop.size() > 0) {
        this->unloadChunkNow(this->chunksQueueDrop.back());
        this->chunksQueueDrop.pop_back();
        return;
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

void World::update() {
    this->chunkBorderDebugRenderer->update();
    this->crosshair->update();
    this->skybox->update();
    this->robot->update();
    this->updateChunkMap(false);
    this->loadChunks();
}

void World::draw(glm::mat4 mat) {
    this->skybox->draw(mat);  // na poczatku - depth offniety

    this->chunkBorderDebugRenderer->draw(mat);
    this->drawChunks(mat);
    this->robot->draw(mat);

    this->crosshair->draw(mat);  // na koncu - depth offniety
}