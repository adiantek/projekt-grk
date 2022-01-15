#pragma once

class Skybox {
    public:
        Skybox();
        ~Skybox();
        void draw();

    private:
        unsigned int cubemapTexture;

        GLuint VAO = 0;
};