#pragma once

#include <Random.hpp>
#include <ResourceLoader.hpp>
#include <opengl.h>

class SimplexNoiseGenerator {
    private:
        double x, y, z;
        int permutationTable[256];
        GLuint vertices;
        GLuint indices;
        GLuint vao;
        GLuint fb;
        GLuint fbTxt;
        GLuint fbBuf;

    public:
        SimplexNoiseGenerator(Random *r, ResourceLoader *res);
        ~SimplexNoiseGenerator();
        void draw(ResourceLoader *res);
        float noiseValues[16 * 16];
};
