#pragma once

#include<Render_Utils.h>
#include<ResourceLoader.hpp>
#include<Water/Simulation.hpp>
#include<Water/EnvironmentMap.hpp>
#include<Water/Caustics.hpp>
#include<glm/ext.hpp>

namespace Water {
    class Surface {
    public:
        Surface(float x, float y, float z, float width, float height, int simulationWidth, int simulationHeight, ResourceLoader* loader);
        ~Surface();
        void draw(glm::mat4 viewMatrix, glm::vec3 cameraPos);
        Simulation simulation;
        Caustics caustics;
    private:
        Core::RenderContext geometry;
        glm::mat4 rotation = glm::eulerAngleX(glm::radians(90.0f));
        glm::vec3 translation;
        unsigned int skybox;
        unsigned int program;
        int uniformCamera;
        int uniformModel;
        int uniformTransformation;
        ResourceLoader* loader;
    };
}