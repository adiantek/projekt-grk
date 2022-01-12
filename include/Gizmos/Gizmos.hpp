#pragma once

#include <glm/ext.hpp>

#include <Render_Utils.h>

class Gizmos {
    public:
        static void init();

        static void cube(glm::vec3 position, glm::vec3 size, glm::vec3 color);
        // static void cube(glm::vec3 position, float size, glm::vec3 color);
        // static void cube(glm::vec3 position, glm::vec3 size);
        // static void cube(glm::vec3 position, float size);
        static void cube(glm::vec3 position);
    private:
        static void drawWireframe(glm::mat4 modelMatrix,Core::RenderContext* renderContext, glm::vec3 color);

        inline static Core::RenderContext* cubeRenderContext;
};