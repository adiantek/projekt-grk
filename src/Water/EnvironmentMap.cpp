#include <Logger.h>

#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Water/EnvironmentMap.hpp>
#include <glm/ext.hpp>

namespace water {
EnvironmentMap::EnvironmentMap(float size, float y, unsigned int textureSize, glm::vec3 lightDirection, float maxDepth) {
    this->size = size;
    this->textureSize = textureSize;
    this->y = y;
    this->setLightDirection(lightDirection);
    this->lightCameraProjectionMatrix = glm::ortho(size / -2.0f, size / 2.0f, size / 2.0f, size / -2.0f, 0.0f, maxDepth);
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
    GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);
    // Check for framebuffer errors
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
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

    this->lightCameraMatrix = this->lightCameraProjectionMatrix 
        * this->lightCameraRotationMatrix 
        * glm::translate(glm::vec3(this->lightCameraTranslation.x - camera->position.x, -this->y, this->lightCameraTranslation.y - camera->position.z));

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

glm::vec3 EnvironmentMap::getLightDirection() {
    return this->lightDirection;
}

void EnvironmentMap::setLightDirection(glm::vec3 newLightDirection) {
    newLightDirection = glm::normalize(newLightDirection);
    this->lightDirection = newLightDirection * glm::mat3(glm::eulerAngleX(glm::radians(90.0f)));
    auto translation = newLightDirection / newLightDirection.y;
    this->lightCameraTranslation = glm::vec2(translation.x, translation.z);
    this->lightCameraRotationMatrix = glm::lookAt(-glm::vec3(translation.x, 0.0f, translation.z), -newLightDirection, glm::vec3(0.0f, 0.0f, 1.0f));
}

void EnvironmentMap::addWorldObject(world::Object3D* object) {
    this->worldObjects.push_back(object);
}

void EnvironmentMap::removeWorldObject(world::Object3D* object) {  // TODO: rewrite this
    world::Object3D* lastObject = this->worldObjects.back();
    for (auto object3D : this->worldObjects) {
        if (object3D == object) {
            object3D = lastObject;
            break;
        }
    }
    this->worldObjects.pop_back();
}

void EnvironmentMap::clearWorldObjects() {
    this->worldObjects.clear();
}

void EnvironmentMap::update() {
    glUseProgram(resourceLoaderExternal->p_environment_map);

    this->lightCameraMatrix = this->lightCameraProjectionMatrix * this->lightCameraRotationMatrix 
        * glm::translate(glm::vec3(this->lightCameraTranslation.x - camera->position.x, -this->y, this->lightCameraTranslation.y - camera->position.z));

    glGetIntegerv(GL_VIEWPORT, this->prevViewport);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glViewport(0, 0, this->textureSize, this->textureSize);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto object3D : this->worldObjects) {
        object3D->draw(this->lightCameraMatrix);
        // glm::mat4 transformation = this->lightCameraMatrix * modelMatrix;
        // glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, (float*)&transformation);
        // glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);
        // Core::DrawContext(context);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(this->prevViewport[0], this->prevViewport[1], this->prevViewport[2], this->prevViewport[3]);
    glUseProgram(0);
}
}  // namespace water