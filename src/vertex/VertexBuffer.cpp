#include <Logger.h>
#include <ResourceLoader.hpp>
#include <vertex/VertexBuffer.hpp>

using namespace vertex;

VertexBuffer::VertexBuffer(VertexFormat *format, uint32_t vertices) {
    this->format = format;
    this->vertices = vertices;
    this->buff = new uint8_t[format->getGPUSize() * vertices];
    this->clear();
}

VertexBuffer::~VertexBuffer() {
    delete[] this->buff;
}

VertexFormat *VertexBuffer::getFormat() {
    return this->format;
}

VertexBuffer *VertexBuffer::pos(float x, float y, float z) {
    float *p = (float *)(this->buffPos + this->format->pos);
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return this;
}

VertexBuffer *VertexBuffer::pos(glm::vec3 vec) {
    return this->pos(vec.x, vec.y, vec.z);
}

VertexBuffer *VertexBuffer::color(float r, float g, float b) {
    return this->color(r, g, b, 1.0f);
}

VertexBuffer *VertexBuffer::color(float r, float g, float b, float a) {
    float *p = (float *)(this->buffPos + this->format->color);
    *p++ = r;
    *p++ = g;
    *p++ = b;
    *p++ = a;
    return this;
}

VertexBuffer *VertexBuffer::tex(float u, float v) {
    float *p = (float *)(this->buffPos + this->format->tex);
    *p++ = u;
    *p++ = v;
    return this;
}

VertexBuffer *VertexBuffer::normal(float x, float y, float z) {
    float *p = (float *)(this->buffPos + this->format->normal);
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return this;
}

VertexBuffer *VertexBuffer::tangent(float x, float y, float z) {
    float *p = (float *)(this->buffPos + this->format->tangent);
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return this;
}

VertexBuffer *VertexBuffer::bitangent(float x, float y, float z) {
    float *p = (float *)(this->buffPos + this->format->bitangent);
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return this;
}

VertexBuffer *VertexBuffer::joint(int jointsIds[3], float jointsWeights[3]) {
    {
        float *p = (float *)(this->buffPos + this->format->jointWeight);
        *p++ = jointsWeights[0];
        *p++ = jointsWeights[1];
        *p++ = jointsWeights[2];
    }
    {
        int *p = (int *)(this->buffPos + this->format->jointID);
        *p++ = jointsIds[0];
        *p++ = jointsIds[1];
        *p++ = jointsIds[2];
    }
    return this;
}

VertexBuffer *VertexBuffer::end() {
    buffPos += this->format->getGPUSize();
    return this;
}

void VertexBuffer::load(const char *name) {
    size_t s = 0;
    char *content = ResourceLoader::readFile(name, &s);
    if (s != format->getGPUSize() * vertices) {
        LOGE("VertexBuffer failed load %s: %zu != %zu", s, format->getGPUSize() * vertices);
        return;
    }
    memcpy(this->buff, content, s);
    free(content);
    this->buffPos = this->buff + s;
}

void VertexBuffer::save(const char *name) {
    uint32_t gotVertices = (uint32_t)(this->buffPos - this->buff) / this->format->getGPUSize();
    if (gotVertices > this->vertices) {
        LOGE("VertexBuffer failed: too many vertices: got %d, expected %d", gotVertices, this->vertices);
        exit(0);
        return;
    }
    ResourceLoader::saveFile(name, (const char *) this->buff, this->buffPos - this->buff);
}

void VertexBuffer::clear() {
    this->buffPos = this->buff;
    for (size_t i = 0; i < format->getGPUSize() * vertices; i++) {
        this->buff[i] = 0;
    }
}

GLuint VertexBuffer::uploadVBO() {
    uint32_t gotVertices = (uint32_t)(this->buffPos - this->buff) / this->format->getGPUSize();
    if (gotVertices > this->vertices) {
        LOGE("VertexBuffer failed: too many vertices: got %d, expected %d", gotVertices, this->vertices);
        exit(0);
        return 0;
    }
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->buffPos - this->buff, this->buff, GL_STATIC_DRAW);
    return vbo;
}

void VertexBuffer::updateVBO(GLuint vbo) {
    uint32_t gotVertices = (uint32_t)(this->buffPos - this->buff) / this->format->getGPUSize();
    if (gotVertices > this->vertices) {
        LOGE("VertexBuffer failed: too many vertices: got %d, expected %d", gotVertices, this->vertices);
        exit(0);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->buffPos - this->buff, this->buff, GL_STATIC_DRAW);
}

void VertexBuffer::configureVAO(GLuint index, GLint size, GLenum type, GLboolean normalized, int32_t pointer) {
    const void *p = (const void *)((const uint8_t *)0 + pointer);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, this->format->getGPUSize(), p);
}

VertexBuffer *VertexBuffer::configurePos(GLuint index) {
    this->configureVAO(index, 3, GL_FLOAT, GL_FALSE, this->getFormat()->pos);
    return this;
}

VertexBuffer *VertexBuffer::configureColor(GLuint index) {
    this->configureVAO(index, 4, GL_FLOAT, GL_FALSE, this->getFormat()->color);
    return this;
}

VertexBuffer *VertexBuffer::configureTex(GLuint index) {
    this->configureVAO(index, 2, GL_FLOAT, GL_FALSE, this->getFormat()->tex);
    return this;
}

VertexBuffer *VertexBuffer::configureNormal(GLuint index) {
    this->configureVAO(index, 3, GL_FLOAT, GL_FALSE, this->getFormat()->normal);
    return this;
}

VertexBuffer *VertexBuffer::configureTangent(GLuint index) {
    this->configureVAO(index, 3, GL_FLOAT, GL_FALSE, this->getFormat()->tangent);
    return this;
}

VertexBuffer *VertexBuffer::configureBitangent(GLuint index) {
    this->configureVAO(index, 3, GL_FLOAT, GL_FALSE, this->getFormat()->bitangent);
    return this;
}

VertexBuffer *VertexBuffer::configureJoint(GLuint indexID, GLuint indexWeight) {
    // TODO @Marcin
    // this->configureVAO(indexID, 3, GL_FLOAT, GL_FALSE, this->getFormat()->jointID);
    // this->configureVAO(indexWeight, 3, GL_FLOAT, GL_FALSE, this->getFormat()->jointWeight);
    return this;
}

uint32_t VertexBuffer::getVertices() {
    return (uint32_t)(this->buffPos - this->buff) / this->format->getGPUSize();
}