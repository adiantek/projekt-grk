#include <Glow/GlowShader.hpp>
#include <Logger.h>
#include <vertex/VertexBuffer.hpp>
#include <ResourceLoader.hpp>
#include <Camera/Camera.hpp>
#include <Fog/Fog.hpp>

using namespace Glow;

GlowShader::GlowShader(GLuint w, GLuint h) {
    glow = this;
    this->w = w;
    this->h = h;

    glGenFramebuffers(1, &this->fb);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

    // GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    // glDrawBuffers(1, &drawBuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Framebuffer not created glow glow glow xD");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    vertex::VertexBuffer vb(&vertex::POS_TEX, 4);
    vb.pos(-1, -1, 0)->tex(0, 0)->end();
    vb.pos(1, -1, 0)->tex(1, 0)->end();
    vb.pos(-1, 1, 0)->tex(0, 1)->end();
    vb.pos(1, 1, 0)->tex(1, 1)->end();

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glUseProgram(resourceLoaderExternal->p_glow);
    this->vbo = vb.uploadVBO();
    vb.configurePos(resourceLoaderExternal->p_glow_attr_pos);
    vb.configureTex(resourceLoaderExternal->p_glow_attr_tex);
    
    const float radius = 3.0f;
    const float quality = 1.0f;

    glUniform1i(resourceLoaderExternal->p_glow_uni_glowTexture, 0);
    glUniform2f(resourceLoaderExternal->p_glow_uni_texelSize, 1.0F / this->w, 1.0F / this->h);
}

GlowShader::~GlowShader() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteFramebuffers(1, &this->fb);
    glDeleteTextures(1, &this->texture);
}

void GlowShader::resize(GLuint w, GLuint h) {
    this->w = w;
    this->h = h;
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    const float radius = 2.0f;
    const float quality = 1.0f;
    glUniform2f(resourceLoaderExternal->p_glow_uni_texelSize, 1.0F / this->w, 1.0F / this->h);
}

void GlowShader::startFB() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
    glViewport(0, 0, this->w, this->h);
}

void GlowShader::stopFB() {
    glBindFramebuffer(GL_FRAMEBUFFER, fog->framebuffer);
    camera->useCameraViewport();
}

void GlowShader::clear() {
    this->startFB();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    this->stopFB();
}

void GlowShader::draw(glm::mat4 mat) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(this->vao);
    glUseProgram(resourceLoaderExternal->p_glow);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

GlowShader *Glow::glow = 0;