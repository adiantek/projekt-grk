#include <Logger.h>

#include <ResourceLoader.hpp>
#include <Time/Time.hpp>
#include <vertex/VertexBuffer.hpp>
#include <world/Chunk.hpp>
#include <world/World.hpp>
#include <Water/Water.hpp>

using namespace world;

Chunk::Chunk(World *world, ChunkPosition pos) {
    this->seed = world->seed;
    this->world = world;
    this->pos = pos;
    this->created = timeExternal->lastFrame;
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->elements);
    // LOGD("Chunk: loading %d %d", pos.coords.x, pos.coords.z);
    this->generate();
}

Chunk::~Chunk() {
    // LOGD("Chunk: unloading %d %d", pos.coords.x, pos.coords.z);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->elements);
    glDeleteVertexArrays(1, &this->vao);
}

Random *Chunk::createChunkRandom() {
    Random *r = new Random(this->seed);
    int64_t xa = r->nextLong();
    int64_t xb = r->nextLong();
    r->setSeed((int64_t)this->pos.coords.x * xa ^ (int64_t)this->pos.coords.z * xb ^ seed);
    return r;
}

void Chunk::generate() {
    int minX = this->pos.coords.x << 4;
    int minZ = this->pos.coords.z << 4;
    float *noise = this->world->noise->draw((float)(this->pos.coords.x), (float)(this->pos.coords.z));

    for (int x = 0; x < 17; x++) {
        for (int y = 0; y < 17; y++) {
            this->heightMap[x * 17 + y] = noise[(x + 1) * 19 + y + 1];
        }
    }
    vertex::VertexBuffer vertices(&vertex::POS_NORMAL_TEX_TANGENT_BITANGENT, 17 * 17);
    for (int x = 0; x <= 16; x++) {
        for (int z = 0; z <= 16; z++) {
            glm::vec3 squares[2][2];
            for (int x1 = 0; x1 < 2; x1++) {
                for (int y1 = 0; y1 < 2; y1++) {
                    float locX = (float)(x + minX + x1);
                    float locY = noise[(z + y1 + 1) * 19 + (x + x1 + 1)] * 128 + 128;
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
        }
    }
    int lineNum = 0;
    int32_t lines[2 * 3 * 16 * 16];
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            lines[lineNum++] = x * 17 + z;
            lines[lineNum++] = x * 17 + z + 1;
            lines[lineNum++] = x * 17 + z + 17;

            lines[lineNum++] = x * 17 + z + 17 + 1;
            lines[lineNum++] = x * 17 + z + 17;
            lines[lineNum++] = x * 17 + z + 1;
        }
    }

    glUseProgram(resourceLoaderExternal->p_shader_tex);
    glBindVertexArray(this->vao);
    vertices.updateVBO(this->vbo);
    vertices.configureTex(resourceLoaderExternal->p_shader_tex_attr_vertexTexCoord);
    vertices.configurePos(resourceLoaderExternal->p_shader_tex_attr_vertexPosition);
    vertices.configureNormal(resourceLoaderExternal->p_shader_tex_attr_vertexNormal);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
}

void Chunk::update() {
}

void Chunk::draw(glm::mat4 mat) {
    double alpha = (timeExternal->lastFrame - this->created) / 1.0;
    if (alpha >= 0.0 && alpha < 1.0) {
        glEnable(GL_BLEND);
        glBlendColor(1.0f, 1.0f, 1.0f, (GLfloat)alpha);
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    }
    glUseProgram(resourceLoaderExternal->p_caustics_shader);
    glUniform1i(resourceLoaderExternal->p_caustics_shader_uni_colorTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_uv);
    glUniform1i(resourceLoaderExternal->p_caustics_shader_uni_normalSampler, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_dummy);
    glUniform1i(resourceLoaderExternal->p_caustics_shader_uni_caustics, 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    glUniform3f(resourceLoaderExternal->p_caustics_shader_uni_lightDirection, lightDir.x, lightDir.y, lightDir.z);
    glUniformMatrix4fv(resourceLoaderExternal->p_caustics_shader_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
    glUniformMatrix4fv(resourceLoaderExternal->p_caustics_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glUniformMatrix4fv(resourceLoaderExternal->p_caustics_shader_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0);  // 1536 = sizeof(lines) / sizeof(int)
    if (alpha >= 0.0 && alpha < 1.0) {
        glDisable(GL_BLEND);
    }
}

void Chunk::drawShadow(glm::mat4 mat) {
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0);  // 1536 = sizeof(lines) / sizeof(int)
}