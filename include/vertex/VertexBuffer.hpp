#pragma once

#include <opengl.h>
#include <vertex/VertexFormats.hpp>

namespace vertex {

class VertexBuffer {
   private:
    VertexFormat *format;
    uint8_t *buff;
    uint32_t vertices;
    uint8_t *buffPos;

   public:
    VertexBuffer(VertexFormat *format, uint32_t vertices);
    ~VertexBuffer();
    VertexFormat *getFormat();
    VertexBuffer *pos(float x, float y, float z);
    VertexBuffer *color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    VertexBuffer *tex(float u, float v);
    VertexBuffer *normal(float x, float y, float z);
    VertexBuffer *tangent(float x, float y, float z);
    VertexBuffer *bitangent(float x, float y, float z);
    VertexBuffer *end();
    void save(const char *name);
    void load(const char *name);
    GLuint uploadVBO();
    uint32_t getVertices();
    void configureVAO(GLuint index, GLint size, GLenum type, GLboolean normalized, int32_t pointer);
};

}  // namespace vertex