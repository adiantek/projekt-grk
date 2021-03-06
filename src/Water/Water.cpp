#include<Water/Water.hpp>
#include<Time/Time.hpp>
#include<ResourceLoader.hpp>

#define DELTATIME 0.032

namespace water {
    Water::Water(float y, float maxDepth, float causticsSize, unsigned int causticsTexureSize, float surfaceSize, unsigned int surfaceTexureSize)
    : caustics(causticsSize, y, causticsTexureSize, maxDepth), surface(surfaceSize, y, surfaceTexureSize) {
        waterObject = this;
        this->y = y;
        glUseProgram(resourceLoaderExternal->p_chunk);
        glUniform1f(resourceLoaderExternal->p_chunk_uni_waterHeight, y);
        glUseProgram(resourceLoaderExternal->p_cubefish);
        glUniform1f(resourceLoaderExternal->p_cubefish_uni_waterHeight, y);
        glUseProgram(resourceLoaderExternal->p_pilotfish);
        glUniform1f(resourceLoaderExternal->p_pilotfish_uni_waterHeight, y);
        glUseProgram(resourceLoaderExternal->p_normal_fish);
        glUniform1f(resourceLoaderExternal->p_normal_fish_uni_waterHeight, y);
        glUseProgram(resourceLoaderExternal->p_instanced_kelp);
        glUniform1f(resourceLoaderExternal->p_instanced_kelp_uni_waterHeight, y);
        glUseProgram(resourceLoaderExternal->p_color_armature);
        glUniform1f(resourceLoaderExternal->p_color_armature_uni_waterHeight, y);
        glUseProgram(resourceLoaderExternal->p_rock);
        glUniform1f(resourceLoaderExternal->p_rock_uni_waterHeight, y);
    }

    Water::~Water() {}

    void Water::draw(glm::mat4 viewMatrix) {
        this->surface.draw(viewMatrix);
    }

    void Water::update() {
        if (timeExternal->lastFrame - this->lastUpdateTime > DELTATIME) {
            this->caustics.update();
            this->surface.update();
            this->lastUpdateTime = (float)glfwGetTime();
        }
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

    float Water::getY() {
        return this->y;
    }
}

#undef DELTATIME

water::Water* waterObject;