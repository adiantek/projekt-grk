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
    VertexBuffer *color(float r, float g, float b);
    VertexBuffer *color(float r, float g, float b, float a);
    VertexBuffer *tex(float u, float v);
    VertexBuffer *normal(float x, float y, float z);
    VertexBuffer *tangent(float x, float y, float z);
    VertexBuffer *bitangent(float x, float y, float z);
    VertexBuffer *joint(int jointsIds[3], float jointsWeights[3]);
    VertexBuffer *end();
    void save(const char *name);
    void load(const char *name);
    void clear();
    GLuint uploadVBO();
    void updateVBO(GLuint vbo);
    uint32_t getVertices();
    void configureVAO(GLuint index, GLint size, GLenum type, GLboolean normalized, int32_t pointer);
    VertexBuffer *configurePos(GLuint index);
    VertexBuffer *configureColor(GLuint index);
    VertexBuffer *configureTex(GLuint index);
    VertexBuffer *configureNormal(GLuint index);
    VertexBuffer *configureTangent(GLuint index);
    VertexBuffer *configureBitangent(GLuint index);
    VertexBuffer *configureJoint(GLuint indexID, GLuint indexWeight);
};

}  // namespace vertex