#pragma once

#include <opengl.h>

#include <Random.hpp>

typedef struct SimplexNoiseLayer {
    float x, y, z;
    int permutationTable[256];
    float weight;
    float scale;
} SimplexNoiseLayer;

class SimplexNoiseGenerator {
   private:
    SimplexNoiseLayer layers[4];
    GLuint vbo;
    GLuint vao;
    GLuint fb;
    GLuint fbTxt;
    GLuint fbBuf;

   public:
    SimplexNoiseGenerator(Random *r);
    ~SimplexNoiseGenerator();
    void draw(float x, float y);
    float noiseValues[16 * 16];
};
