#include <Logger.h>

#include <vertex/VertexBuffer.hpp>

using namespace vertex;

VertexBuffer::VertexBuffer(VertexFormat *format, uint32_t vertices) {
    this->format = format;
    this->vertices = vertices;
    this->buff = new uint8_t[format->getGPUSize() * vertices];
    this->buffPos = this->buff;
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

VertexBuffer *VertexBuffer::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    uint8_t *p = (uint8_t *)(this->buffPos + this->format->color);
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

VertexBuffer *VertexBuffer::end() {
    buffPos += this->format->getGPUSize();
    return this;
}

GLuint VertexBuffer::uploadVBO() {
    uint32_t gotVertices = (uint32_t)(this->buffPos - this->buff) / this->format->getGPUSize();
    if (gotVertices != this->vertices) {
        LOGE("VertexBuffer failed: too many or too low vertices: got %d, expected %d", gotVertices, this->vertices);
        return 0;
    }
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->format->getGPUSize() * this->vertices, this->buff, GL_STATIC_DRAW);
    return vbo;
}

void VertexBuffer::configureVAO(GLuint index, GLint size, GLenum type, GLboolean normalized, int32_t pointer) {
    const void *p = (const void *)((const uint8_t *)0 + pointer);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, this->format->getGPUSize(), p);
}