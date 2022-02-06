#include <Logger.h>

#include <Physics/Physics.hpp>
#include <ResourceLoader.hpp>
#include <Time/Time.hpp>
#include <Water/Water.hpp>
#include <vertex/VertexBuffer.hpp>
#include <world/Chunk.hpp>
#include <world/World.hpp>

using namespace world;

Chunk::Chunk(World *world, ChunkPosition pos, float *noise) {
    this->seed = world->seed;
    this->world = world;
    this->pos = pos;
    this->created = timeExternal->lastFrame;
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->elements);
    // LOGD("Chunk: loading %d %d", pos.coords.x, pos.coords.z);
    this->generate(noise);
}

Chunk::~Chunk() {
    // LOGD("Chunk: unloading %d %d", pos.coords.x, pos.coords.z);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->elements);
    glDeleteVertexArrays(1, &this->vao);
    delete this->rigidBody;
    if (this->minFishYCalculated) {
        glDeleteBuffers(1, &this->vboLines);
        glDeleteVertexArrays(1, &this->vaoLines);
    }
}

Random *Chunk::createChunkRandom() {
    Random *r = new Random(this->seed);
    int64_t xa = r->nextLong();
    int64_t xb = r->nextLong();
    r->setSeed((int64_t)this->pos.coords.x * xa ^ (int64_t)this->pos.coords.z * xb ^ seed);
    return r;
}

void Chunk::generate(float *noise) {
    int minX = this->pos.coords.x << 4;
    int minZ = this->pos.coords.z << 4;
    // float *noise = this->world->noise->readNoise();//this->world->noise->draw((float)(this->pos.coords.x), (float)(this->pos.coords.z));
    // this->world->noise->debugNoise((float)(this->pos.coords.x), (float)(this->pos.coords.z));
    for (int i = 0; i < 19 * 19; i++) {
        noise[i] = noise[i] * 128 + 128;
    }
    this->maxY = 0;
    for (int x = 0; x < 17; x++) {
        for (int y = 0; y < 17; y++) {
            this->heightMap[x * 17 + y] = noise[(x + 1) * 19 + y + 1];
            if (this->heightMap[x * 17 + y] > this->maxY) {
                this->maxY = this->heightMap[x * 17 + y];
            }
        }
    }

    vertex::VertexBuffer vertices(&vertex::POS_NORMAL_TEX_TANGENT_BITANGENT, 17 * 17);
    float vert[17 * 17 * 3];
    for (int x = 0; x <= 16; x++) {
        for (int z = 0; z <= 16; z++) {
            glm::vec3 squares[2][2];
            for (int x1 = 0; x1 < 2; x1++) {
                for (int y1 = 0; y1 < 2; y1++) {
                    float locX = (float)(x + minX + x1);
                    float locY = noise[(z + y1 + 1) * 19 + (x + x1 + 1)];
                    float locZ = (float)(z + minZ + y1);
                    squares[x1][y1] = glm::vec3(locX, locY, locZ);
                }
            }

            glm::vec3 deltaPos1 = squares[0][1] - squares[0][0];
            glm::vec3 deltaPos2 = squares[1][0] - squares[0][0];

            glm::vec2 deltaUV1 = glm::vec2(deltaPos1.x, deltaPos1.z);
            glm::vec2 deltaUV2 = glm::vec2(deltaPos2.x, deltaPos2.z);

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

            glm::vec3 normal = glm::normalize(glm::cross(deltaPos1, deltaPos2));
            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            vertices.tex((float)x, (float)z);
            vertices.pos(squares[0][0]);
            vertices.normal(normal.x, normal.y, normal.z);
            vertices.tangent(tangent.x, tangent.y, tangent.z);
            vertices.bitangent(bitangent.x, bitangent.y, bitangent.z);

            // vertices.color(x / 16.0f, (this->heightMap[z * 17 + x] + 1.0f) / 2.0f, z / 16.0f);
            vertices.end();

            vert[3 * (x * 17 + z)] = squares[0][0].x;
            vert[3 * (x * 17 + z) + 1] = squares[0][0].y;
            vert[3 * (x * 17 + z) + 2] = squares[0][0].z;
        }
    }
    int n = 0;
    int32_t indices[2 * 3 * 16 * 16];
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            indices[n++] = x * 17 + z;
            indices[n++] = x * 17 + z + 1;
            indices[n++] = x * 17 + z + 17;

            indices[n++] = x * 17 + z + 17 + 1;
            indices[n++] = x * 17 + z + 17;
            indices[n++] = x * 17 + z + 1;
        }
    }
    physx::PxTransform transform = physx::PxTransform(0.0f, 0.0f, 0.0f);
    physx::PxTriangleMeshGeometry geometry = physicsObject->createTriangleGeometry(&vertices, indices, 2 * 16 * 16);
    this->rigidBody = new physics::RigidBody(true, transform, geometry, (world::Object3D *)this, 0.5f, 0.5f, 0.0001f);
    geometry.triangleMesh->release();

    glUseProgram(resourceLoaderExternal->p_chunk);
    glBindVertexArray(this->vao);
    vertices.updateVBO(this->vbo);
    vertices.configureTex(resourceLoaderExternal->p_chunk_attr_vertexTexCoord);
    vertices.configurePos(resourceLoaderExternal->p_chunk_attr_vertexPosition);
    vertices.configureNormal(resourceLoaderExternal->p_chunk_attr_vertexNormal);
    vertices.configureTangent(resourceLoaderExternal->p_chunk_attr_vertexTangent);
    vertices.configureBitangent(resourceLoaderExternal->p_chunk_attr_vertexBitangent);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Chunk::update() {
    if (!this->minFishYCalculated) {
        float allowFishAbove = this->maxY + 8;  // maxY sasiednich chunkow
        for (int32_t x = this->pos.coords.x - 1; x <= this->pos.coords.x + 1; x++) {
            for (int32_t z = this->pos.coords.z - 1; z <= this->pos.coords.z + 1; z++) {
                Chunk *c = this->world->getChunkAt(ChunkPosition(x, z));
                if (c) {
                    if (allowFishAbove < c->maxY) {
                        allowFishAbove = c->maxY;
                    }
                } else {
                    allowFishAbove = 256;
                }
            }
        }
        if (allowFishAbove != 256) {
            this->minFishYCalculated = true;
            this->allowFishAbove = allowFishAbove;

            int32_t sx = this->pos.coords.x << 4;
            int32_t sz = this->pos.coords.z << 4;

            vertex::VertexBuffer vb(&vertex::POS_COLOR, 12);

            vb.pos((float)(sx + 0), allowFishAbove, (float)(sz + 0))->color(1, 0, 0)->end();
            vb.pos((float)(sx + 0), allowFishAbove, (float)(sz + 16))->color(1, 0, 0)->end();

            vb.pos((float)(sx + 0), allowFishAbove, (float)(sz + 0))->color(1, 0, 0)->end();
            vb.pos((float)(sx + 16), allowFishAbove, (float)(sz + 0))->color(1, 0, 0)->end();

            vb.pos((float)(sx + 0), allowFishAbove, (float)(sz + 0))->color(1, 0, 0)->end();
            vb.pos((float)(sx + 16), allowFishAbove, (float)(sz + 16))->color(1, 0, 0)->end();

            vb.pos((float)(sx + 0), allowFishAbove, (float)(sz + 16))->color(1, 0, 0)->end();
            vb.pos((float)(sx + 16), allowFishAbove, (float)(sz + 0))->color(1, 0, 0)->end();

            vb.pos((float)(sx + 0), allowFishAbove, (float)(sz + 16))->color(1, 0, 0)->end();
            vb.pos((float)(sx + 16), allowFishAbove, (float)(sz + 16))->color(1, 0, 0)->end();

            vb.pos((float)(sx + 16), allowFishAbove, (float)(sz + 0))->color(1, 0, 0)->end();
            vb.pos((float)(sx + 16), allowFishAbove, (float)(sz + 16))->color(1, 0, 0)->end();

            glGenVertexArrays(1, &this->vaoLines);
            glUseProgram(resourceLoaderExternal->p_simple_color_shader);
            glBindVertexArray(this->vaoLines);
            this->vboLines = vb.uploadVBO();
            vb.configureColor(resourceLoaderExternal->p_simple_color_shader_attr_vertexColor);
            vb.configurePos(resourceLoaderExternal->p_simple_color_shader_attr_vertexPosition);
        }
    }
}

void Chunk::prepareRendering(glm::mat4 mat) {
    glUseProgram(resourceLoaderExternal->p_chunk);

    const GLint textures = 3;
    GLint arr[textures];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    glUniform1iv(resourceLoaderExternal->p_chunk_uni_colorTexture, textures, arr);
    glActiveTexture(GL_TEXTURE0 + arr[0]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_albedo);
    glActiveTexture(GL_TEXTURE0 + arr[1]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_albedo);
    glActiveTexture(GL_TEXTURE0 + arr[2]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_albedo);
    
    for (GLint i = 0; i < textures; i++) arr[i] += textures;
    glUniform1iv(resourceLoaderExternal->p_chunk_uni_normalSampler, textures, arr);
    glActiveTexture(GL_TEXTURE0 + arr[0]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_normal);
    glActiveTexture(GL_TEXTURE0 + arr[1]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_normal);
    glActiveTexture(GL_TEXTURE0 + arr[2]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_normal);
    
    for (GLint i = 0; i < textures; i++) arr[i] += textures;
    glUniform1iv(resourceLoaderExternal->p_chunk_uni_depthMap, textures, arr);
    glActiveTexture(GL_TEXTURE0 + arr[0]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_height);
    glActiveTexture(GL_TEXTURE0 + arr[1]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_height);
    glActiveTexture(GL_TEXTURE0 + arr[2]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_height);
    
    for (GLint i = 0; i < textures; i++) arr[i] += textures;
    glUniform1iv(resourceLoaderExternal->p_chunk_uni_roughnessMap, textures, arr);
    glActiveTexture(GL_TEXTURE0 + arr[0]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_roughness);
    glActiveTexture(GL_TEXTURE0 + arr[1]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_roughness);
    glActiveTexture(GL_TEXTURE0 + arr[2]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_roughness);
    
    for (GLint i = 0; i < textures; i++) arr[i] += textures;
    glUniform1iv(resourceLoaderExternal->p_chunk_uni_aoMap, textures, arr);
    glActiveTexture(GL_TEXTURE0 + arr[0]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_ao);
    glActiveTexture(GL_TEXTURE0 + arr[1]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_ao);
    glActiveTexture(GL_TEXTURE0 + arr[2]);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_ao);

    glUniform1i(resourceLoaderExternal->p_chunk_uni_caustics, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());

    glUniform1i(resourceLoaderExternal->p_chunk_uni_envMap, 16);
    glActiveTexture(GL_TEXTURE0 + 16);
    glBindTexture(GL_TEXTURE_2D, waterObject->getEnvironmentMap());

    glUniformMatrix4fv(resourceLoaderExternal->p_chunk_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
    glUniformMatrix4fv(resourceLoaderExternal->p_chunk_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glUniformMatrix4fv(resourceLoaderExternal->p_chunk_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));
}

void Chunk::drawTerrain(glm::mat4 mat) {
    double alpha = (timeExternal->lastFrame - this->created) / 1.0;
    if (alpha >= 0.0 && alpha < 1.0) {
        glEnable(GL_BLEND);
        glBlendColor(1.0f, 1.0f, 1.0f, (GLfloat)alpha);
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    }
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0);  // 1536 = sizeof(lines) / sizeof(int)

    if (alpha >= 0.0 && alpha < 1.0) {
        glDisable(GL_BLEND);
    }
}

void Chunk::draw(glm::mat4 mat) {
    // TODO fishes, stones, etc.
}

void Chunk::drawShadow(glm::mat4 mat) {
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0);  // 1536 = sizeof(lines) / sizeof(int)
}