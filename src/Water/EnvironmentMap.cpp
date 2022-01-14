#include<Water/EnvironmentMap.hpp>
#include<Logger.h>
#include<glm/ext.hpp>
#include<ResourceLoader.hpp>
#include<Camera/Camera.hpp>

namespace water {
    EnvironmentMap::EnvironmentMap(float size, unsigned int textureSize, float y, glm::vec3 lightDirection, float maxDepth) {
        this->size = size;
        this->textureSize = textureSize;
        this->y = y;
        this->lightDirection = lightDirection;
        this->lightCameraProjectionMatrix = glm::ortho(size / -2.0f, size / 2.0f, size / 2.0f, size / -2.0f, 0.0f, maxDepth);
        this->lightCameraRotationMatrix = glm::eulerAngleX(glm::radians(90.0f)); // TODO use light direction to define rotation
        this->lightCameraTranslation = glm::vec3(0.0f, 0.0f, 0.0f); // TODO use light direction to define transition
        // Create framebuffer
        glGenFramebuffers(1, &this->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        // Create depthbuffer
        glGenRenderbuffers(1, &this->depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, this->depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->textureSize, this->textureSize);
        // Bind depthbuffer to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthbuffer);
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
        // Check for framebuffer errors
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("Framebuffer not created");
        }
        // Bind previous framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    EnvironmentMap::~EnvironmentMap() {
        glDeleteTextures(1, &this->texture);
        glDeleteRenderbuffers(1, &this->depthbuffer);
        glDeleteFramebuffers(1, &this->framebuffer);
    }

    void EnvironmentMap::useFramebuffer() {
        glUseProgram(resourceLoaderExternal->p_environment_map);

        glm::vec3 translation = glm::vec3(-camera->position.x, -this->y, -camera->position.z);
        this->lightCameraMatrix = this->lightCameraProjectionMatrix * this->lightCameraRotationMatrix * glm::translate(translation);

        glGetIntegerv(GL_VIEWPORT, this->prevViewport);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glViewport(0, 0, this->textureSize, this->textureSize);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void EnvironmentMap::stopUsingFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(this->prevViewport[0], this->prevViewport[1], this->prevViewport[2], this->prevViewport[3]);
        glUseProgram(0);
    }

    void EnvironmentMap::drawObject(Core::RenderContext context, glm::mat4 modelMatrix) {
        glm::mat4 transformation = this->lightCameraMatrix * modelMatrix;
        glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, (float*)&transformation);
        glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);
        Core::DrawContext(context);
    }

    unsigned int EnvironmentMap::getMapTexture() {
        return this->texture;
    }

    glm::mat4 EnvironmentMap::getLightCamera() {
        return this->lightCameraMatrix;
    }
}