#pragma once

#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Animator/Joint.hpp>

class Gizmos {
    public:
        static void init();

        static void cube(glm::vec3 position, glm::vec3 size, glm::vec3 color);
        static void cube(glm::vec3 position, float size, glm::vec3 color);
        static void cube(glm::vec3 position, glm::vec3 size);
        static void cube(glm::vec3 position, float size);
        static void cube(glm::vec3 position);

        static void line(glm::vec3 start, glm::vec3 end);
        static void line(glm::vec3 start, glm::vec3 end, glm::vec3 color);

        static void joint(glm::vec3 offset, Animator::Joint* joint);

        static void grid(float size);
        static void grid(float size, glm::vec3 color);

        static void printJointsTree(Animator::Joint* joint);
        static void printJointsTree(Animator::Joint* joint, std::string prefix);
};