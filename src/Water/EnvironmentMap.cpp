#include<Water/EnvironmentMap.hpp>
#include<Logger.h>
#include<glm/ext.hpp>

namespace Water {
    EnvironmentMap::EnvironmentMap(float size, int textureSize, ResourceLoader* loader) {
        this->size = size;
        this->textureSize = textureSize;
        this->program = loader->p_environment_map;
        this->uniformModel = loader->p_environment_map_uni_modelMatrix;
        this->uniformTransformation = loader->p_environment_map_uni_transformation;
        this->projectionMatrix = glm::ortho(-(this->size / 2.0f), this->size / 2.0f, this->size / 2.0f, -(this->size / 2.0f), 0.0001f, 1000.0f);
        this->rotationMatrix = glm::eulerAngleX(-glm::radians(90.0f));
        // Create framebuffer
        glGenFramebuffers(1, &this->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        // Create texture
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->textureSize, this->textureSize, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Bind texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        // Create depthbuffer
        glGenRenderbuffers(1, &this->depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, this->depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->textureSize, this->textureSize);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthbuffer);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("Framebuffer not created");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    EnvironmentMap::~EnvironmentMap() {}

    void EnvironmentMap::useFramebuffer(glm::vec3 transition) {
        glUseProgram(this->program);

        this->transformationMatrix = this->projectionMatrix * this->rotationMatrix * glm::translate(transition);

        glGetIntegerv(GL_VIEWPORT, this->prevViewport);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glViewport(0, 0, this->textureSize, this->textureSize);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void EnvironmentMap::stopUsingFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(this->prevViewport[0], this->prevViewport[1], this->prevViewport[2], this->prevViewport[3]);
        glUseProgram(0);
    }

    void EnvironmentMap::drawObject(Core::RenderContext context, glm::mat4 modelMatrix) {
        glm::mat4 transformation = this->transformationMatrix * modelMatrix;
        glUniformMatrix4fv(this->uniformTransformation, 1, GL_FALSE, (float*)&transformation);
        glUniformMatrix4fv(this->uniformModel, 1, GL_FALSE, (float*)&modelMatrix);
        Core::DrawContext(context);
    }
}