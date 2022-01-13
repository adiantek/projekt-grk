#include<Water/Caustics.hpp>
#include<Logger.h>
#include<ResourceLoader.hpp>
#include<Camera/Camera.hpp>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

namespace water {
    Caustics::Caustics(float size, unsigned int textureSize, float y, unsigned int heightMap, unsigned int normalMap) 
    : environmentMap(size, textureSize, y) {
        this->size = size;
        this->textureSize = textureSize;
        this->y = y;
        this->heightMap = heightMap;
        this->normalMap = normalMap;
        this->geometry.initPlane(size, size, textureSize, textureSize);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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

    Caustics::~Caustics() {
        glDeleteTextures(1, &this->texture);
        glDeleteRenderbuffers(1, &this->depthbuffer);
        glDeleteFramebuffers(1, &this->framebuffer);
    }

    void Caustics::render() {
        glUseProgram(resourceLoaderExternal->p_caustics);

        int prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glViewport(0, 0, this->textureSize, this->textureSize);

        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->heightMap);
        glUniform1i(resourceLoaderExternal->p_caustics_uni_depthMap, 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, this->environmentMap.getMapTexture());
        glUniform1i(resourceLoaderExternal->p_caustics_uni_environmentMap, 1);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, this->normalMap);
        glUniform1i(resourceLoaderExternal->p_caustics_uni_normalMap, 2);


        glm::mat4 transformation = glm::ortho(-(this->size / 2.0f), this->size / 2.0f, this->size / 2.0f, -(this->size / 2.0f), 0.0f, 50.0f) * glm::eulerAngleX(glm::radians(90.0f));
        glm::mat4 rotation = glm::eulerAngleX(glm::radians(-90.0f));

        glm::mat4 translation1 = glm::translate(glm::vec3(-camera->position.x, -this->y, -camera->position.z));

        glUniform1f(resourceLoaderExternal->p_caustics_uni_deltaEnvTexture, 1.0f / (float) this->textureSize);
        glUniform3f(resourceLoaderExternal->p_caustics_uni_light, 0.0f, 0.0f, -1.0f);
        glUniformMatrix4fv(resourceLoaderExternal->p_caustics_uni_transformation, 1, GL_FALSE, (float*)&transformation);
        glUniformMatrix4fv(glGetUniformLocation(resourceLoaderExternal->p_caustics, "rotation"), 1, GL_FALSE, (float*)&rotation);
        glUniformMatrix4fv(glGetUniformLocation(resourceLoaderExternal->p_caustics, "translation"), 1, GL_FALSE, (float*)&translation1);

        Core::DrawContext(this->geometry);

        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
        glUseProgram(0);
    }

    unsigned int Caustics::getTexture() {
        return this->texture;
    }

    glm::mat4 Caustics::getLightCamera() {
        return this->environmentMap.getLightCamera();
    }
}
