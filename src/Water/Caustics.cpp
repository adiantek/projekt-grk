#include<Water/Caustics.hpp>
#include<Logger.h>
#include<ResourceLoader.hpp>
#include<Camera/Camera.hpp>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

namespace water {
    Caustics::Caustics(float size, float y, unsigned int textureSize) 
    : environmentMap(size, y, textureSize * 4), simulation(size, textureSize) {
        this->size = size;
        this->y = y;
        this->textureSize = textureSize * 4;
        this->geometry.initPlane(size, size, this->textureSize, this->textureSize);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Bind texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
        GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &drawBuffer);
        // Check for framebuffer errors
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("Framebuffer not created");
        }
        // Bind previous framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        this->simulation.generateRandomWaves();
    }

    Caustics::~Caustics() {
        glDeleteTextures(1, &this->texture);
        glDeleteRenderbuffers(1, &this->depthbuffer);
        glDeleteFramebuffers(1, &this->framebuffer);
    }

    void Caustics::update() {
        this->simulation.update();

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
        glBindTexture(GL_TEXTURE_2D, this->simulation.getHeightMap());
        glUniform1i(resourceLoaderExternal->p_caustics_uni_heightMap, 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, this->environmentMap.getMapTexture());
        glUniform1i(resourceLoaderExternal->p_caustics_uni_environmentMap, 1);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, this->simulation.getNormalMap());
        glUniform1i(resourceLoaderExternal->p_caustics_uni_normalMap, 2);


        glm::mat4 model = glm::translate(glm::vec3(camera->position.x, this->y, camera->position.z)) * glm::eulerAngleX(glm::radians(-90.0f));
        glm::mat4 lightCamera = this->environmentMap.getLightCamera();

        glUniform1f(resourceLoaderExternal->p_caustics_uni_height, this->y);
        glm::vec3 lightDirection = this->environmentMap.getLightDirection();
        glUniform3f(resourceLoaderExternal->p_caustics_uni_lightDirection, lightDirection.x, lightDirection.y, lightDirection.z);
        glUniformMatrix4fv(resourceLoaderExternal->p_caustics_uni_transformation, 1, GL_FALSE, (float*)&lightCamera);
        glUniformMatrix4fv(resourceLoaderExternal->p_caustics_uni_modelMatrix, 1, GL_FALSE, (float*)&model);

        Core::DrawContext(this->geometry);

        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
        glUseProgram(0);
    }

    unsigned int Caustics::getCausticsMap() {
        return this->texture;
    }

    unsigned int Caustics::getHeightMap() {
        return this->simulation.getHeightMap();
    }

    unsigned int Caustics::getNormalMap() {
        return this->simulation.getNormalMap();
    }

    glm::mat4 Caustics::getLightCamera() {
        return this->environmentMap.getLightCamera();
    }

    void Caustics::useFramebuffer() {
        this->environmentMap.useFramebuffer();
    }

    void Caustics::stopUsingFramebuffer() {
        this->environmentMap.stopUsingFramebuffer();
    }

    void Caustics::drawObject(Core::RenderContext context, glm::mat4 modelMatrix) {
        this->environmentMap.drawObject(context, modelMatrix);
    }

    void Caustics::addWorldObject(world::Object3D* object) {
        this->environmentMap.addWorldObject(object);
    }

    void Caustics::removeWorldObject(world::Object3D* object) {
        this->environmentMap.removeWorldObject(object);
    }

    void Caustics::clearWorldObjects() {
        this->environmentMap.clearWorldObjects();
    }
}
