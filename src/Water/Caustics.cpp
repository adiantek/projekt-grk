#include<Water/Caustics.hpp>
#include<Logger.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

namespace Water {
    Caustics::Caustics(float size, int textureSize, unsigned int heightMap, unsigned int normalMap, ResourceLoader *loader) 
    : environmentMap(size, textureSize, loader) {
        this->geometry.initPlane(size, size, textureSize, textureSize);
        this->textureSize = textureSize;
        this->program = loader->p_caustics;
        this->uniformDeltaEnvTexture = loader->p_caustics_uni_deltaEnvTexture;
        this->uniformDepthMap = loader->p_caustics_uni_depthMap;
        this->uniformEnvironmentMap = loader->p_caustics_uni_environmentMap;
        this->uniformLight = loader->p_caustics_uni_light;
        this->uniformNormalMap = loader->p_caustics_uni_normalMap;
        this->uniformTransformation = loader->p_caustics_uni_transformation;
        this->size = size;

        this->normalMap = normalMap;
        this->depthMap = heightMap;

        glGenFramebuffers(1, &this->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        // Create texture
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->textureSize * 3, this->textureSize * 3, 0, GL_RGBA, GL_FLOAT, 0);
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
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->textureSize * 3, this->textureSize * 3);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthbuffer);

        //glEnable(GL_BLEND);
        //glBlendFuncSeparatei(this->framebuffer, GL_ONE, GL_ONE, GL_ONE, GL_ZERO); TODO:


        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("Framebuffer not created");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    Caustics::~Caustics() {}
    void Caustics::render() {
        glUseProgram(this->program);

        int prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glViewport(0, 0, this->textureSize * 3, this->textureSize * 3);

        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->depthMap);
        glUniform1i(this->uniformDepthMap, 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, this->environmentMap.texture);
        glUniform1i(this->uniformEnvironmentMap, 1);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, this->normalMap);
        glUniform1i(this->uniformNormalMap, 2);

        glm::mat4 transformation = glm::ortho(-(this->size / 2.0f), this->size / 2.0f, this->size / 2.0f, -(this->size / 2.0f), 0.0001f, 30.0f) * glm::eulerAngleX(glm::radians(90.0f)) * glm::translate(-glm::vec3(0.0f, 9.0f, 0.0f));
        glm::mat4 rotation = glm::eulerAngleX(glm::radians(-90.0f));

        glUniform1f(this->uniformDeltaEnvTexture, 1.0f / (float) this->textureSize);
        glUniform3f(this->uniformLight, 0.0f, 0.0f, -1.0f);
        glUniformMatrix4fv(this->uniformTransformation, 1, GL_FALSE, (float*)&transformation);
        glUniformMatrix4fv(glGetUniformLocation(this->program, "rotation"), 1, GL_FALSE, (float*)&rotation);

        Core::DrawContext(this->geometry);

        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
        glUseProgram(0);
    }
    glm::mat4 Caustics::getLightCamera() {
        return glm::ortho(-(this->size / 2.0f), this->size / 2.0f, this->size / 2.0f, -(this->size / 2.0f), 0.0001f, 30.0f) * glm::eulerAngleX(glm::radians(90.0f)) * glm::translate(-glm::vec3(0.0f, 9.0f, 0.0f));
    }
}
