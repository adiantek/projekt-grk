#pragma once

#include <Random.hpp>
#include <ResourceLoader.hpp>
#include <opengl.h>

class SimplexNoiseGenerator {
    private:
        double x, y, z;
        int permutationTable[256];
        GLuint buffer;

    public:
        SimplexNoiseGenerator(Random *r);
        void draw(ResourceLoader *res);
};
