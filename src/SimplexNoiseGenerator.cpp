#include <SimplexNoiseGenerator.hpp>
#include <Render_Utils.h>
#include <Logger.h>

SimplexNoiseGenerator::SimplexNoiseGenerator(Random *r, ResourceLoader *res) {
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

    float vertexArray[16] = {
            -1, -1, 0, 1,
            1, -1, 0, 1,
            -1, 1, 0, 1,
            1, 1, 0, 1
    };
    
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertexArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(res->p_simplex_attr_vertexPosition);
    glVertexAttribPointer(res->p_simplex_attr_vertexPosition, 4, GL_FLOAT, false, 0, 0);
    glBindVertexArray(0);
    
    int indexArray[4] = {0, 1, 2, 3};
    glGenBuffers(1, &this->indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 4, indexArray, GL_STATIC_DRAW);

    glGenFramebuffers(1, &this->fb);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glGenTextures(1, &this->fbTxt);
    glBindTexture(GL_TEXTURE_2D, this->fbTxt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fbTxt, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SimplexNoiseGenerator::~SimplexNoiseGenerator() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vertices);
    glDeleteVertexArrays(1, &this->indices);
    glDeleteFramebuffers(1, &this->fb);
    glDeleteTextures(1, &this->fbTxt);
}

void SimplexNoiseGenerator::draw(ResourceLoader *res) {
    // we can skip glClear, bcuz GL_BLEND is disabled

    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);

    glUseProgram(res->p_simplex);
    glUniform1iv(res->p_simplex_uni_p, 256, this->permutationTable);
    glUniform1f(res->p_simplex_uni_scale, 10.0F);
    glUniform1f(res->p_simplex_uni_alpha, 1.0F);
    
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

    glReadPixels(0, 0, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, this->noiseValues);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // LOGD("%.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f",
    //     this->noiseValues[0], this->noiseValues[1], this->noiseValues[2], this->noiseValues[3],
    //     this->noiseValues[4], this->noiseValues[5], this->noiseValues[6], this->noiseValues[7]);
}