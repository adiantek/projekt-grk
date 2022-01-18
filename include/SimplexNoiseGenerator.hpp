#pragma once

#include <Random.hpp>
#include <opengl.h>

typedef struct SimplexNoiseLayer {

} SimplexNoiseLayer;

class SimplexNoiseGenerator {
    private:
        double x, y, z;
        int permutationTable[256];
        GLuint vbo;
        GLuint vao;
        GLuint fb;
        GLuint fbTxt;
        GLuint fbBuf;

    public:
        SimplexNoiseGenerator(Random *r);
        ~SimplexNoiseGenerator();
        void draw();
        float noiseValues[16 * 16];
};
