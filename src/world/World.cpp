#include <Logger.h>

#include <Time/Time.hpp>
#include <world/ChunkPositionComparator.hpp>
#include <world/World.hpp>
#include <utils/Gizmos.hpp>
#include <ResourceLoader.hpp>

using namespace world;

#define VIEW_DISTANCE 16

World::World(int64_t seed) {
    worldObject = this;
    this->seed = seed;
    this->chunkBorderDebugRenderer = new ChunkBorderDebugRenderer();
    this->crosshair = new Crosshair();
    this->skybox = new cam::Skybox();
    this->robot = new entity::Robot();
    Random r(seed);
    this->noise = new SimplexNoiseGenerator(&r, 0.2);
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

void World::loadChunkNow(ChunkPosition pos, float *noise) {
    Chunk *ch = new Chunk(this, pos, noise);
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
    if (this->noise->isBusy()) {
        float *noise = this->noise->readNoise();
        if (noise) {
            ChunkPosition pos = this->chunksQueue.front();
            if (pos.id == this->noiseValues.id) {
                this->loadChunkNow(pos, noise);
                this->chunksQueue.pop_front();
            } else {
                bool found = false;
                for (std::deque<ChunkPosition>::reverse_iterator it = this->chunksQueue.rbegin(); it != this->chunksQueue.rend(); ++it) {
                    if ((*it).id == this->noiseValues.id) {
                        found = true;
                        if (this->chunksQueue.back().id == this->noiseValues.id) {
                            this->chunksQueue.pop_back();
                        } else {
                            *it = this->chunksQueue.back();
                            this->chunksQueue.pop_back();
                        }
                        break;
                    }
                }
                if (found) {
                    this->loadChunkNow(this->noiseValues, noise);
                }
                std::sort(this->chunksQueue.begin(), this->chunksQueue.end(), ChunkPositionComparator_comparator);
            }
        }
    } else {
        if (this->chunksQueue.size() > 0) {
            ChunkPosition pos = this->chunksQueue.front();
            this->noise->draw((float)(pos.coords.x), (float)(pos.coords.z));
            this->noiseValues = pos;
            // this->loadChunkNow(this->chunksQueue.front());
            // this->chunksQueue.pop_front();
        }
    }
    if (this->chunksQueueDrop.size() > 0) {
        this->unloadChunkNow(this->chunksQueueDrop.back());
        this->chunksQueueDrop.pop_back();
    }
}

Chunk *World::getChunkAt(ChunkPosition pos) {
    auto c = this->chunks.find(pos.id);
    if (c == this->chunks.end()) {
        return 0;
    }
    std::pair<const uint64_t, Chunk *> pair = *c;
    return pair.second;
}

float World::getHeightAt(int32_t x, int32_t z) {
    Chunk *c = this->getChunkAt(ChunkPosition(x >> 4, z >> 4));
    if (!c) {
        return NAN;
    }
    return c->getHeightAt(x, z);
}

float World::getHeightAt(float x, float z) {
    Chunk *c = this->getChunkAt(ChunkPosition(glm::vec3(x, 0, z)));
    if (!c) {
        return NAN;
    }
    return c->getHeightAt(x, z);
}

void World::updateChunks() {
    for (auto &it : this->chunks) {
        Chunk *ch = it.second;
        ch->update();
    }
}

void World::drawChunks(glm::mat4 mat) {
    bool first = true;
    for (auto &it : this->chunks) {
        Chunk *ch = it.second;
        if (first) {
            Chunk::prepareRendering(mat);
            first = false;
        }
        ch->drawTerrain(mat);
    }
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

    ResourceLoader *res = resourceLoaderExternal;
    
    // kelp:
    this->kelp.upload();
    glUseProgram(res->p_instanced_kelp);
    glUniformMatrix4fv(res->p_instanced_kelp_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->kelp.getTexture());
    glUniform1i(res->p_instanced_kelp_uni_matrices, 0);
    glUniform1i(res->p_instanced_kelp_uni_texAlbedo, 1);
    glUniform1i(res->p_instanced_kelp_uni_textureSize, this->kelp.getTextureSize());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, res->tex_foliage_kelp_kelp_albedo);
    for (auto &mesh : res->m_foliage_kelp->getMeshes()) {
        glBindVertexArray(mesh->getRenderContext()->vertexArray);
        glDrawElementsInstanced(GL_TRIANGLES, mesh->getRenderContext()->size, GL_UNSIGNED_INT, (void *)0, this->kelp.getInstances());
    }

    this->crosshair->draw(mat);  // na koncu - depth offniety

    glDisable(GL_CULL_FACE);
}

void World::drawShadow(glm::mat4 mat) {
    this->drawShadowChunks(mat);
    this->robot->drawShadow(mat);
}

bool World::chunksLoaded(glm::vec3 pos) {
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if(!this->getChunkAt(ChunkPosition(pos + glm::vec3(i * 16.0f, 0.0f, j * 16.0f)))) {
                return false;
            }
        }
    }
    return true;
}

World *worldObject;