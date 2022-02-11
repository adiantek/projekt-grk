#pragma once

#include <stdbool.h>
#include <opengl.h>

namespace utils {
class MatrixTextureArray {
   private:
    GLsizei size = 4;
    size_t used = 0;
    float *memory = 0;
    size_t **indices = 0;
    bool *dirty = 0;
    GLuint texture;
    void resize();
   public:
    MatrixTextureArray();
    ~MatrixTextureArray();
    void addMatrix(float *mat, size_t *out_index);
    void removeMatrix(size_t index);
    size_t getInstances();
    void upload();
    GLuint getTexture();
    GLsizei getTextureSize();
};
}  // namespace utils
