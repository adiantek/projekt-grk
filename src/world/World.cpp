#include <Logger.h>

#include <world/ChunkPositionComparator.hpp>
#include <world/World.hpp>

using namespace world;

#define VIEW_DISTANCE 2

World::World() {
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
    this->robot = new entity::Robot();

    for (int i = 0; i < 20 * 20; i++) {
        this->matrix[i] = false;
    }

    this->updateChunks(true);
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

void World::loadChunk(ChunkPosition pos) {
    int32_t x = pos.coords.x;
    int32_t z = pos.coords.z;
    LOGD("load: %d %d", x, z);
    int32_t rx = x + 10;
    int32_t rz = z + 10;
    if (rx >= 0 && rx <= 19 && rz >= 0 && rz <= 19) {
        int32_t ci = rx * 20 + rz;
        this->matrix[ci] = true;
    }
    this->chunksQueue.push_back(pos);
}

void World::unloadChunk(ChunkPosition pos) {
    int32_t x = pos.coords.x;
    int32_t z = pos.coords.z;
    LOGD("unload: %d %d", x, z);
    int32_t rx = x + 10;
    int32_t rz = z + 10;
    if (rx >= 0 && rx <= 19 && rz >= 0 && rz <= 19) {
        int32_t ci = rx * 20 + rz;
        this->matrix[ci] = true;
    }
    for (std::vector<ChunkPosition>::reverse_iterator it=this->chunksQueue.rbegin(); it!=this->chunksQueue.rend(); ++it) {
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

void World::updateChunks(bool firstLoad) {
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
                this->loadChunk(ChunkPosition(x, z));
            }
            if (!this->shouldChunkBeLoaded(x - diffX, z - diffZ, currX, currZ, viewDistance)) {
                this->unloadChunk(ChunkPosition(x - diffX, z - diffZ));
            }
        }
    }
    ChunkPositionComparator_translate = glm::vec2(pos.x, pos.z) / 16.0F;
    std::sort(this->chunksQueue.begin(), this->chunksQueue.end(), ChunkPositionComparator_comparator);
    for (std::vector<ChunkPosition>::iterator it=this->chunksQueue.begin(); it!=this->chunksQueue.end(); ++it) {
        ChunkPosition pos = *it;
        LOGD("chunk to load: %d %d", pos.coords.x, pos.coords.z);
    }
    for (int32_t x = 0; x < 20; x++) {
        for (int32_t z = 0; z < 20; z++) {
            printf("%s", this->matrix[x * 20 + z] ? "#" : x == 10 && z == 10 ? "+" : x == 10 ? "-" : z == 10 ? "|" : " ");
        }
        printf("\n");
    }
}

void World::update() {
    this->chunkBorderDebugRenderer->update();
    this->crosshair->update();
    this->skybox->update();
    this->robot->update();
    this->updateChunks(false);
}

void World::draw(glm::mat4 mat) {
    this->skybox->draw(mat);  // na poczatku - depth offniety

    this->chunkBorderDebugRenderer->draw(mat);
    this->robot->draw(mat);

    this->crosshair->draw(mat);  // na koncu - depth offniety
}