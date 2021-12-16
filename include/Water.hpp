#pragma once

#include<opengl.h>
#include<glad/glad.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Render_Utils.h"
#include<ResourceLoader.hpp>

class Water {
public:
    Water(float x, float y, float z, float width, float height, int simulationWidth, int simulationHeight, ResourceLoader* loader);
    ~Water();
    void stepSimulation();
    void addDrop(float x, float y, float radius, float strength);
private:
    void renderSimulation();
    unsigned int frameBuffer;
    unsigned int frameBufferWidth;
    unsigned int frameBufferHeight;
    unsigned int textures[2];
    Core::RenderContext geometry;
    Core::RenderContext dummyGeometry;
    glm::mat4 rotation = glm::eulerAngleX(90.0f);
    glm::vec3 translation;
    ResourceLoader* loader;
};