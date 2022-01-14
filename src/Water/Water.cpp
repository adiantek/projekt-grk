#include<Water/Water.hpp>

namespace water {
    Water::Water(float size, float y, unsigned int texureSize)
    : caustics(size, y, texureSize)
    , simulation {
        Simulation(size, texureSize / 2, glm::vec2(size - 0.1f, 0.0f)),
        Simulation(size, texureSize / 2, glm::vec2(size - 0.1f, size - 0.1f)),
        Simulation(size, texureSize / 2, glm::vec2(0.1f - size, 0.1f - size)),
        Simulation(size, texureSize / 2, glm::vec2(0.1f - size, size - 0.1f)),
        Simulation(size, texureSize / 2, glm::vec2(size - 0.1f, 0.1f - size)),
        Simulation(size, texureSize / 2, glm::vec2(0.0f, size - 0.1f)),
        Simulation(size, texureSize / 2, glm::vec2(0.0f, 0.1f - size)),
        Simulation(size, texureSize / 2, glm::vec2(0.1f - size, 0.0f))
    }
    , surface {
        Surface(size, y, texureSize, caustics.getHeightMap(), caustics.getNormalMap()),
        Surface(size, y, texureSize / 2, simulation[0].getHeightMap(), simulation[0].getNormalMap(), glm::vec2(size - 0.1f, 0.0f)),
        Surface(size, y, texureSize / 2, simulation[1].getHeightMap(), simulation[1].getNormalMap(), glm::vec2(size - 0.1f, size - 0.1f)),
        Surface(size, y, texureSize / 2, simulation[2].getHeightMap(), simulation[2].getNormalMap(), glm::vec2(0.1f - size, 0.1f - size)),
        Surface(size, y, texureSize / 2, simulation[3].getHeightMap(), simulation[3].getNormalMap(), glm::vec2(0.1f - size, size - 0.1f)),
        Surface(size, y, texureSize / 2, simulation[4].getHeightMap(), simulation[4].getNormalMap(), glm::vec2(size - 0.1f, 0.1f - size)),
        Surface(size, y, texureSize / 2, simulation[5].getHeightMap(), simulation[5].getNormalMap(), glm::vec2(0.0f, size - 0.1f)),
        Surface(size, y, texureSize / 2, simulation[6].getHeightMap(), simulation[6].getNormalMap(), glm::vec2(0.0f, 0.1f - size)),
        Surface(size, y, texureSize / 2, simulation[7].getHeightMap(), simulation[7].getNormalMap(), glm::vec2(0.1f - size, 0.0f)),
    } {}

    Water::~Water() {}

    void Water::draw() {
        this->caustics.render();
        for(int i=0; i<8; ++i)
		    this->simulation[i].simulate();
        for(int i=0; i<9; ++i)
		    this->surface[i].draw();
    }
    
    void Water::useFramebuffer() {
        this->caustics.useFramebuffer();
    }

    void Water::drawObject(Core::RenderContext context, glm::mat4 modelMatrix) {
        this->caustics.drawObject(context, modelMatrix);
    }

    void Water::stopUsingFramebuffer() {
        this->caustics.stopUsingFramebuffer();
    }

    glm::mat4 Water::getLightCamera() {
        return this->caustics.getLightCamera();
    }

    unsigned int Water::getCausticsMap() {
        return this->caustics.getCausticsMap();
    }
}