#include <Camera/Camera.hpp>
#include <Water/Water.hpp>
#include <ResourceLoader.hpp>
#include <Fog/Fog.hpp>
#include <glm/ext.hpp>

Fog::Fog(unsigned int width, unsigned int height, float maxDepth) {
    this->maxDepth = maxDepth;
    this->width = width;
    this->height = height;

    // Create framebuffer
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

    // Create depthbuffer
    // glEnable(GL_DEPTH_TEST);

    glGenRenderbuffers(1, &this->depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, this->width, this->height);

    // Bind depthbuffer to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthbuffer);

    // Create color texture
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create depth texture
    glGenTextures(1, &this->depthTexture);
    glBindTexture(GL_TEXTURE_2D, this->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Bind texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture, 0);

    GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);

    // Check for framebuffer errors
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Framebuffer not created fog fog fog");
    }
    // Bind previous framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    float quadVao[] ={
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f,0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &this->rectVAO);
    glGenBuffers(1, &this->rectVBO);
    glBindVertexArray(this->rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVao), &quadVao, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

Fog::~Fog() {
    glDeleteTextures(1, &this->texture);
    glDeleteTextures(1, &this->depthTexture);
    glDeleteRenderbuffers(1, &this->depthbuffer);
    glDeleteFramebuffers(1, &this->framebuffer);
    glDeleteVertexArrays(1, &this->rectVAO);
    glDeleteBuffers(1, &this->rectVBO);
}

void Fog::useFramebuffer() {
    glUseProgram(resourceLoaderExternal->p_underwater_fog_shader);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Fog::stopUsingFramebuffer() {
    glUseProgram(resourceLoaderExternal->p_underwater_fog_shader);

    glUniform1f(resourceLoaderExternal->p_underwater_fog_shader_uni_distanceToSurface, waterObject->getY() - camera->position.y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(resourceLoaderExternal->p_underwater_fog_shader_uni_screenTexture, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->depthTexture);
    glUniform1i(resourceLoaderExternal->p_underwater_fog_shader_uni_depthTexture, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    camera->useCameraViewport();

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(this->rectVAO);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

Fog *fog;