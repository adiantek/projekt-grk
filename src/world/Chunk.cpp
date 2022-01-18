#include <world/Chunk.hpp>
#include <Logger.h>
#include <world/World.hpp>
#include <vertex/VertexBuffer.hpp>
#include <ResourceLoader.hpp>
#include <Time/Time.hpp>

using namespace world;

Chunk::Chunk(World *world, ChunkPosition pos) {
    this->seed = world->seed;
    this->world = world;
    this->pos = pos;
    this->created = timeExternal->lastFrame;
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->elements);
    LOGD("Chunk: loading %d %d", pos.coords.x, pos.coords.z);
    this->generate();
}

Chunk::~Chunk() {
    LOGD("Chunk: unloading %d %d", pos.coords.x, pos.coords.z);
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
    for (int i = 0; i < 17 * 17; i++) {
        this->heightMap[i] = noise[i];
    }
    vertex::VertexBuffer vertices(&vertex::POS_TEX, 17 * 17);
    int32_t lines[2 * 3 * 16 * 16];
    for (int x = 0; x <= 16; x++) {
        for (int z = 0; z <= 16; z++) {
            vertices.tex((float)x, (float)z);
            vertices.pos((float)(x + minX), noise[z * 17 + x] * 128 + 128, (float)(z + minZ));
            // vertices.color(x / 16.0f, (noise[z * 17 + x] + 1.0f) / 2.0f, z / 16.0f);
            vertices.end();
        }
    }
    int lineNum = 0;
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

    glUseProgram(resourceLoaderExternal->p_simple_tex_shader);
    glBindVertexArray(this->vao);
    vertices.updateVBO(this->vbo);
    vertices.configureTex(resourceLoaderExternal->p_simple_tex_shader_attr_vertexTex);
    vertices.configurePos(resourceLoaderExternal->p_simple_tex_shader_attr_vertexPosition);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
}

void Chunk::update() {
}

void Chunk::draw(glm::mat4 mat) {
    double alpha = (timeExternal->lastFrame - this->created) / 1.0;
    if (alpha >= 0.0 && alpha < 1.0) {
        glEnable(GL_BLEND);
        glBlendColor(1.0f, 1.0f, 1.0f, (GLfloat) alpha);
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    }
    glUseProgram(resourceLoaderExternal->p_simple_tex_shader);
    glUniform1i(resourceLoaderExternal->p_simple_tex_shader_uni_textureSampler, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_uv);
    glUniformMatrix4fv(resourceLoaderExternal->p_simple_tex_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 1536, GL_UNSIGNED_INT, 0); // 1536 = sizeof(lines) / sizeof(int)
    if (alpha >= 0.0 && alpha < 1.0) {
        glDisable(GL_BLEND);
    }
}