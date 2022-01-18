#include<Water/Water.hpp>
#include<Time/Time.hpp>
#include<ResourceLoader.hpp>

#define DELTATIME 0.025

namespace water {
    Water::Water(float y, float maxDepth, float causticsSize, unsigned int causticsTexureSize, float surfaceSize, unsigned int surfaceTexureSize)
    : caustics(causticsSize, y, causticsTexureSize, maxDepth), surface(surfaceSize, y, surfaceTexureSize) {
        waterObject = this;
        glUseProgram(resourceLoaderExternal->p_caustics_shader);
        glUniform1f(resourceLoaderExternal->p_caustics_shader_uni_waterHeight, y);
        glUseProgram(0);
    }

    Water::~Water() {}

    void Water::draw(glm::mat4 viewMatrix) {
        this->surface.draw(viewMatrix);
    }

    void Water::update() {
        if (timeExternal->lastFrame - this->lastUpdateTime > DELTATIME) {
            this->caustics.update();
            this->surface.update();
            this->lastUpdateTime = timeExternal->lastFrame;
        }
    }
    
    void Water::useFramebuffer() {
        if (timeExternal->lastFrame - this->lastUpdateTime > DELTATIME)
            this->caustics.useFramebuffer();
    }

    void Water::drawObject(Core::RenderContext context, glm::mat4 modelMatrix) {
        if (timeExternal->lastFrame - this->lastUpdateTime > DELTATIME)
            this->caustics.drawObject(context, modelMatrix);
    }

    void Water::stopUsingFramebuffer() {
        if (timeExternal->lastFrame - this->lastUpdateTime > DELTATIME)
            this->caustics.stopUsingFramebuffer();
    }

    glm::mat4 Water::getLightCamera() {
        return this->caustics.getLightCamera();
    }

    unsigned int Water::getCausticsMap() {
        return this->caustics.getCausticsMap();
    }

    void Water::addWorldObject(world::Object3D* object) {
        this->caustics.addWorldObject(object);
    }

    void Water::removeWorldObject(world::Object3D* object) {
        this->caustics.removeWorldObject(object);
    }

    void Water::clearWorldObjects() {
        this->caustics.clearWorldObjects();
    }
}

#undef DELTATIME

water::Water* waterObject;