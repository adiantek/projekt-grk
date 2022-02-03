#include <Logger.h>
#include <Render_Utils.h>

#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <vertex/VertexBuffer.hpp>
#include <Fog/Fog.hpp>

SimplexNoiseGenerator::SimplexNoiseGenerator(Random *r) {
    double scale = 0.20;  // im mniejsza wartosc tym teren lagodniejszy
    double weight = 1.0 / ((1 << 4) - 1);

    uint8_t permutationTable[4 * 256];

    for (int l = 0; l < 4; l++) {
        double x = r->nextDouble() * 256.0;
        double y = r->nextDouble() * 256.0;
        double z = r->nextDouble() * 256.0;
        for (int i = 0; i < 256; i++) {
            permutationTable[l * 256 + i] = (uint8_t)i;
        }
        for (int i = 0; i < 256; i++) {
            int j = r->nextInt(256 - i);
            uint8_t k = permutationTable[l * 256 + i];
            permutationTable[l * 256 + i] = permutationTable[l * 256 + j + i];
            permutationTable[l * 256 + j + i] = k;
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

    glGenTextures(1, &this->permutationTex);
    glBindTexture(GL_TEXTURE_2D, this->permutationTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 256, 4, 0, GL_RED, GL_UNSIGNED_BYTE, permutationTable);

    vertex::VertexBuffer vertices(&vertex::POS_TEX, 4);
    vertices.pos(-1, -1, 0)->tex(-3.0f / 32.0f, -3.0f / 32.0f)->end();
    vertices.pos(1, -1, 0)->tex(35.0f / 32.0f, -3.0f / 32.0f)->end();
    vertices.pos(-1, 1, 0)->tex(-3.0f / 32.0f, 35.0f / 32.0f)->end();
    vertices.pos(1, 1, 0)->tex(35.0f / 32.0f, 35.0f / 32.0f)->end();

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 19, 19, 0, GL_RED, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fbTxt, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glUseProgram(res->p_simplex);
    glUniform1i(res->p_simplex_uni_p, 0);

    glGenBuffers(1, &this->pbo);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, this->pbo);
    glBufferData(GL_PIXEL_PACK_BUFFER, 19 * 19 * 4, 0, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    this->sync = 0;
}

SimplexNoiseGenerator::~SimplexNoiseGenerator() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteFramebuffers(1, &this->fb);
    glDeleteTextures(1, &this->fbTxt);
    glDeleteTextures(1, &this->permutationTex);
}

bool SimplexNoiseGenerator::isBusy() {
    return this->sync != 0;
}

float *SimplexNoiseGenerator::readNoise() {
    GLenum res = glClientWaitSync(this->sync, 0, 0);
    if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, this->pbo);
        glGetBufferSubData(GL_PIXEL_PACK_BUFFER, 0, sizeof(this->noiseValues), this->noiseValues);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        this->sync = 0;
        return this->noiseValues;
    } else if (res == GL_TIMEOUT_EXPIRED) {
        return 0;
    } else {
        LOGD("glClientWaitSync: %d", res);
        return 0;
    }
}

void SimplexNoiseGenerator::draw(float x, float y) {
    ResourceLoader *res = resourceLoaderExternal;

    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, this->pbo);
    glViewport(0, 0, 19, 19);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE);
    glBindVertexArray(this->vao);
    glUseProgram(res->p_simplex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->permutationTex);

    for (int l = 0; l < 4; l++) {
        glUniform1f(res->p_simplex_uni_layer, (float)l);
        glUniform1f(res->p_simplex_uni_scale, this->layers[l].scale);
        glUniform2f(res->p_simplex_uni_translation, x + this->layers[l].x, y + this->layers[l].y);
        glBlendColor(0.0f, 0.0f, 0.0f, this->layers[l].weight);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glReadPixels(0, 0, 19, 19, GL_RED, GL_FLOAT, 0);
    this->sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glDisable(GL_BLEND);
    glBindFramebuffer(GL_FRAMEBUFFER, fog->framebuffer);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    camera->useCameraViewport();
}

void SimplexNoiseGenerator::debugNoise(float x, float y) {
    LOGD("Debug noise: [%.1f, %.1f]", x, y);
    this->draw(x, y);
    for (int y = 0; y < 19; y++) {
        for (int x = 0; x < 19; x++) {
            printf("%.3f   ", this->noiseValues[y * 19 + x]);
        }
        printf("\n");
    }
}