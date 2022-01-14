#pragma once

#include<Render_Utils.h>
#include<glm/ext.hpp>

namespace water {
    class Surface {
    public:
        Surface(float size, float y, unsigned int textureSize, unsigned int heightMap, unsigned int normalMap);
        ~Surface();
        void draw();
    private:
        float size;
        float y;
        unsigned int heightMap;
        unsigned int normalMap;
        Core::RenderContext geometry;
        unsigned int skybox;
        glm::mat4 rotation = glm::eulerAngleX(glm::radians(90.0f));
    };
}