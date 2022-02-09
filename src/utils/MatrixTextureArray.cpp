#include <utils/MatrixTextureArray.hpp>
#include <stdlib.h>
#include <string.h>
#include <Logger.h>

using namespace utils;

MatrixTextureArray::MatrixTextureArray() {
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->size, this->size, 0, GL_RGBA, GL_FLOAT, 0);

    this->memory = (float *)malloc(sizeof(float) * this->size * this->size * 4);
    this->indices = (size_t **)malloc(sizeof(size_t *) * this->size * this->size / 4);
    this->dirty = (bool *)malloc(sizeof(bool) * this->size * this->size / 4);
}

MatrixTextureArray::~MatrixTextureArray() {
    free(this->memory);
    glDeleteTextures(1, &this->texture);
}

void MatrixTextureArray::resize() {
    this->size <<= 1;
    this->memory = (float *)realloc(this->memory, sizeof(float) * this->size * this->size * 4);
    this->indices = (size_t **)realloc(this->indices, sizeof(size_t *) * this->size * this->size / 4);
    this->dirty = (bool *)realloc(this->dirty, sizeof(bool) * this->size * this->size / 4);
    for (size_t i = 0; i < this->size * this->size / 4; i++) {
        this->dirty[i] = true;
    }
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->size, this->size, 0, GL_RGBA, GL_FLOAT, 0);
}

void MatrixTextureArray::addMatrix(float *mat, size_t *out_index) {
    if (used >= this->size * this->size / 4) {
        this->resize();
    }
    this->indices[this->used] = out_index;
    memcpy(this->memory + (this->used * 16), mat, sizeof(float) * 16);
    *out_index = this->used;
    this->dirty[this->used] = true;
    this->used++;
}

void MatrixTextureArray::removeMatrix(size_t index) {
    if (index == this->used - 1) {
        this->dirty[index] = 0;
        this->used--;
    } else {
        memcpy(this->memory + (index * 16), this->memory + ((this->used - 1) * 16), sizeof(float) * 16);
        this->indices[index] = this->indices[this->used - 1];
        *(this->indices[index]) = index;
        this->dirty[index] = true;
        this->used--;
    }
}

void MatrixTextureArray::upload() {
    bool active = false;
    bool uploadPending = false;
    size_t from = 0;
    size_t row = 0;
    for (size_t i = 0; i < this->used; i++) {
        if (this->dirty[i]) {
            this->dirty[i] = false;
            if (!active) {
                active = true;
                glBindTexture(GL_TEXTURE_2D, this->texture);
            }
            if (!uploadPending) {
                from = i;
                row = (i * 4) / this->size;
                uploadPending = true;
            } else {
                if ((i * 4) / this->size != row) {
                    glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint) ((from * 4) % this->size), (GLint) ((from * 4) / this->size), (GLsizei) (4 * (i - from)), 1, GL_RGBA, GL_FLOAT, this->memory + from * 16);
                    from = i;
                    row = (i * 4) / this->size;
                }
            }
        } else {
            if (uploadPending) {
                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint) ((from * 4) % this->size), (GLint) ((from * 4) / this->size), (GLsizei) (4 * (i - from)), 1, GL_RGBA, GL_FLOAT, this->memory + from * 16);
                uploadPending = false;
            }
        }
    }
    if (uploadPending) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint) ((from * 4) % this->size), (GLint) ((from * 4) / this->size), (GLsizei) (4 * (this->used - from)), 1, GL_RGBA, GL_FLOAT, this->memory + from * 16);
        uploadPending = false;
    }
}

size_t MatrixTextureArray::getInstances() {
    return this->used;
}

GLuint MatrixTextureArray::getTexture() {
    return this->texture;
}

GLsizei MatrixTextureArray::getTextureSize() {
    return this->size;
}