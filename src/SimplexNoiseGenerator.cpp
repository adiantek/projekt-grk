#include <Logger.h>
#include <Render_Utils.h>

#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <vertex/VertexBuffer.hpp>

SimplexNoiseGenerator::SimplexNoiseGenerator(Random *r) {
    double scale = 0.25; // im mniejsza wartosc tym teren lagodniejszy
    double weight = 1.0 / ((1 << 4) - 1);

    for (int l = 0; l < 4; l++) {
        double x = r->nextDouble() * 256.0;
        double y = r->nextDouble() * 256.0;
        double z = r->nextDouble() * 256.0;
        for (int i = 0; i < 256; i++) {
            this->layers[l].permutationTable[i] = i;
        }
        for (int i = 0; i < 256; i++) {
            int j = r->nextInt(256 - i);
            int k = this->layers[l].permutationTable[i];
            this->layers[l].permutationTable[i] = this->layers[l].permutationTable[j + i];
            this->layers[l].permutationTable[j + i] = k;
        }
        this->layers[l].weight = (float)weight;
        this->layers[l].scale = (float)scale;

        LOGD("Created new simplex layer: %.3f %.3f %3.f, w: %.3f, s: %.3f",
             x, y, z, this->layers[l].weight, this->layers[l].scale);

        this->layers[l].x = (float)(x / this->layers[l].scale);
        this->layers[l].y = (float)(y / this->layers[l].scale);
        this->layers[l].z = (float)(z / this->layers[l].scale);

        scale /= 2.0;
        weight *= 2.0;
    }

    vertex::VertexBuffer vertices(&vertex::POS_TEX, 4);
    vertices.pos(-1, -1, 0)->tex(-1.0f / 32.0f, -1.0f / 32.0f)->end();
    vertices.pos(1, -1, 0)->tex(33.0f / 32.0f, -1.0f / 32.0f)->end();
    vertices.pos(-1, 1, 0)->tex(-1.0f / 32.0f, 33.0f / 32.0f)->end();
    vertices.pos(1, 1, 0)->tex(33.0f / 32.0f, 33.0f / 32.0f)->end();

    ResourceLoader *res = resourceLoaderExternal;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    this->vbo = vertices.uploadVBO();
    vertices.configurePos(res->p_simplex_attr_pos);
    vertices.configureTex(res->p_simplex_attr_tex);

    glGenFramebuffers(1, &this->fb);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glGenTextures(1, &this->fbTxt);
    glBindTexture(GL_TEXTURE_2D, this->fbTxt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 17, 17, 0, GL_RED, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fbTxt, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SimplexNoiseGenerator::~SimplexNoiseGenerator() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteFramebuffers(1, &this->fb);
    glDeleteTextures(1, &this->fbTxt);
}

float *SimplexNoiseGenerator::draw(float x, float y) {
    ResourceLoader *res = resourceLoaderExternal;

    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glViewport(0, 0, 17, 17);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE);
    glBindVertexArray(this->vao);
    glUseProgram(res->p_simplex);

    for (int l = 0; l < 4; l++) {
        glUniform1iv(res->p_simplex_uni_p, 256, this->layers[l].permutationTable);
        glUniform1f(res->p_simplex_uni_scale, this->layers[l].scale);
        glUniform2f(res->p_simplex_uni_translation, x + this->layers[l].x, y + this->layers[l].y);
        glBlendColor(0.0f, 0.0f, 0.0f, this->layers[l].weight);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glReadPixels(0, 0, 17, 17, GL_RED, GL_FLOAT, this->noiseValues);

    glDisable(GL_BLEND);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    camera->useCameraViewport();

    // for (int y = 0; y < 17; y++) {
    //     for (int x = 0; x < 17; x++) {
    //         printf("%.3f   ", this->noiseValues[y * 17 + x]);
    //     }
    //     printf("\n");
    // }

    return this->noiseValues;
}

void SimplexNoiseGenerator::debugNoise(float x, float y) {
    LOGD("Debug noise: [%.1f, %.1f]", x, y);
    this->draw(x, y);
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            printf("%.3f   ", this->noiseValues[y * 17 + x]);
        }
        printf("\n");
    }
}