#pragma once

#include <opengl.h>

#include <Random.hpp>

typedef struct SimplexNoiseLayer {
    float x, y, z;
    // int permutationTable[256];
    float weight;
    float scale;
} SimplexNoiseLayer;

class SimplexNoiseGenerator {
   private:
    SimplexNoiseLayer layers[4];
    float noiseValues[19 * 19];
    GLuint vbo;
    GLuint vao;
    GLuint fb;
    GLuint fbTxt;
    GLuint permutationTex;
    GLuint pbo;
    GLsync sync;

   public:
    SimplexNoiseGenerator(Random *r);
    ~SimplexNoiseGenerator();
    /**
     * @param x offset x
     * @param y offset y
     */
    void draw(float x, float y);
    /**
     * @brief DO NOT FREE/DELETE!
     */
    float *readNoise();
    bool isBusy();

    void debugNoise(float x, float y);
};
