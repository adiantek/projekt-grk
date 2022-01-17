#include<Water/Water.hpp>
#include<Time/Time.hpp>

#define BIAS 0.15f
#define DELTATIME 0.025

namespace water {
    Water::Water(float size, float y, unsigned int texureSize)
    : caustics(size, y, texureSize)
    , simulation {
        Simulation(size, texureSize / 2, glm::vec2(size - BIAS, 0.0f)),
        Simulation(size, texureSize / 2, glm::vec2(size - BIAS, size - BIAS)),
        Simulation(size, texureSize / 2, glm::vec2(BIAS - size, BIAS - size)),
        Simulation(size, texureSize / 2, glm::vec2(BIAS - size, size - BIAS)),
        Simulation(size, texureSize / 2, glm::vec2(size - BIAS, BIAS - size)),
        Simulation(size, texureSize / 2, glm::vec2(0.0f, size - BIAS)),
        Simulation(size, texureSize / 2, glm::vec2(0.0f, BIAS - size)),
        Simulation(size, texureSize / 2, glm::vec2(BIAS - size, 0.0f))
    }
    , surface {
        Surface(size, y, texureSize, caustics.getHeightMap(), caustics.getNormalMap()),
        Surface(size, y, texureSize / 2, simulation[0].getHeightMap(), simulation[0].getNormalMap(), glm::vec2(size - BIAS, 0.0f)),
        Surface(size, y, texureSize / 2, simulation[1].getHeightMap(), simulation[1].getNormalMap(), glm::vec2(size - BIAS, size - BIAS)),
        Surface(size, y, texureSize / 2, simulation[2].getHeightMap(), simulation[2].getNormalMap(), glm::vec2(BIAS - size, BIAS - size)),
        Surface(size, y, texureSize / 2, simulation[3].getHeightMap(), simulation[3].getNormalMap(), glm::vec2(BIAS - size, size - BIAS)),
        Surface(size, y, texureSize / 2, simulation[4].getHeightMap(), simulation[4].getNormalMap(), glm::vec2(size - BIAS, BIAS - size)),
        Surface(size, y, texureSize / 2, simulation[5].getHeightMap(), simulation[5].getNormalMap(), glm::vec2(0.0f, size - BIAS)),
        Surface(size, y, texureSize / 2, simulation[6].getHeightMap(), simulation[6].getNormalMap(), glm::vec2(0.0f, BIAS - size)),
        Surface(size, y, texureSize / 2, simulation[7].getHeightMap(), simulation[7].getNormalMap(), glm::vec2(BIAS - size, 0.0f)),
    } {
        waterObject = this;
    }

    Water::~Water() {}

    void Water::draw(glm::mat4 viewMatrix) {
        for(auto & object : this->surface)
            object.draw(viewMatrix);
    }

    void Water::update() {
        if (timeExternal->lastFrame - this->lastUpdateTime > DELTATIME) {
            this->caustics.update();
            for(auto & object : this->surface)
                object.update();
            for(auto & object : this->simulation)
                object.update();
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

#undef BIAS
#undef DELTATIME

water::Water* waterObject;