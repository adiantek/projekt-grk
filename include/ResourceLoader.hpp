#pragma once

#include <opengl.h>
#include <stdbool.h>

class ResourceLoader {
    public:
        ResourceLoader();
        ~ResourceLoader();
        /**
         * return true if everything loaded
         */
        bool loadResources();

        int totalResources = 0;
        int loadedResources = 0;

        // textures
        GLuint txt_grid = 0;
        GLuint txt_gridColor = 0;
        GLuint txt_earth = 0;
        GLuint txt_earth2 = 0;
        GLuint txt_moon = 0;
        GLuint txt_ship = 0;
        GLuint txt_earthNormal = 0;
        GLuint txt_asteroidNormal = 0;
        GLuint txt_shipNormal = 0;
        GLuint txt_asteroid = 0;
    
    private:
        int totalResourcesCounter = 0;
        bool all_loaded = false;
        double startFrame = 0;
        bool canLoadNextResource();
        void loadTextures();
        void loadTexture(const char *name, GLuint *out);
};