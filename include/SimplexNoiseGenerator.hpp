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
    float noiseValues[17 * 17];
    GLuint vbo;
    GLuint vao;
    GLuint fb;
    GLuint fbTxt;

   public:
    SimplexNoiseGenerator(Random *r);
    ~SimplexNoiseGenerator();
    /**
     * @brief DO NOT FREE/DELETE!
     * 
     * @param x offset x
     * @param y offset y
     * @return float* size 17 * 17, values from -0.9506353780500628 to 0.9442659147897063
     */
    float *draw(float x, float y);

    void debugNoise(float x, float y);
};
