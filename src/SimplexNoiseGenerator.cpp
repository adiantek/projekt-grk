#include <SimplexNoiseGenerator.hpp>
#include <Render_Utils.h>
#include <ResourceLoader.hpp>
#include <Logger.h>
#include <Camera/Camera.hpp>
#include <vertex/VertexBuffer.hpp>

GLuint fbTxtTest = 0;

SimplexNoiseGenerator::SimplexNoiseGenerator(Random *r) {
    this->x = r->nextDouble() * 256.0;
    this->y = r->nextDouble() * 256.0;
    this->z = r->nextDouble() * 256.0;
    for (int i = 0; i < 256; i++) {
        this->permutationTable[i] = i;
    }
    for (int i = 0; i < 256; i++) {
        int j = r->nextInt(256 - i);
        int k = this->permutationTable[i];
        this->permutationTable[i] = this->permutationTable[j + i];
        this->permutationTable[j + i] = k;
    }

    vertex::VertexBuffer vertices(&vertex::POS_TEX, 4);
    vertices.pos(-1, -1, 0)->tex(-1.0f/32.0f, -1.0f/32.0f)->end();
    vertices.pos(1, -1, 0)->tex(31.0f/32.0f, -1.0f/32.0f)->end();
    vertices.pos(-1, 1, 0)->tex(-1.0f/32.0f, 31.0f/32.0f)->end();
    vertices.pos(1, 1, 0)->tex(31.0f/32.0f, 31.0f/32.0f)->end();

    ResourceLoader *res = resourceLoaderExternal;
    
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    this->vbo = vertices.uploadVBO();
    vertices.configureVAO(res->p_simplex_attr_pos, 3, GL_FLOAT, GL_FALSE, vertices.getFormat()->pos);
    vertices.configureVAO(res->p_simplex_attr_tex, 2, GL_FLOAT, GL_FALSE, vertices.getFormat()->tex);
    
    glGenFramebuffers(1, &this->fb);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glGenTextures(1, &this->fbTxt);
    glBindTexture(GL_TEXTURE_2D, this->fbTxt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 16, 16, 0, GL_RED, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fbTxt, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SimplexNoiseGenerator::~SimplexNoiseGenerator() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteFramebuffers(1, &this->fb);
    glDeleteTextures(1, &this->fbTxt);
}

void SimplexNoiseGenerator::draw() {
    ResourceLoader *res = resourceLoaderExternal;

    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glViewport(0, 0, 16, 16);

    glUseProgram(res->p_simplex);
    glUniform1iv(res->p_simplex_uni_p, 256, this->permutationTable);
    glUniform1f(res->p_simplex_uni_scale, 1.0F);

    //glEnable(GL_BLEND);
    //glBlendColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glBlendFunc(GL_CONSTANT_ALPHA, GL_CONSTANT_ALPHA);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glReadPixels(0, 0, 16, 16, GL_RED, GL_FLOAT, this->noiseValues);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    camera->useCameraViewport();

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            printf("%.3f   ", this->noiseValues[y * 16 + x]);
        }
        printf("\n");
    }
}