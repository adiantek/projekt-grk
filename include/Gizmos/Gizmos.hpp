#pragma once

#include <string>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Animator/Joint.hpp>
#include <Gizmos/Line.hpp>

class Gizmos {
    public:
        static void init();

        static void cube(std::string id, glm::vec3 position, glm::vec3 size, glm::vec3 color);
        static void cube(std::string id, glm::vec3 position, float size, glm::vec3 color);
        static void cube(std::string id, glm::vec3 position, glm::vec3 size);
        static void cube(std::string id, glm::vec3 position, float size);
        static void cube(std::string id, glm::vec3 position);

        static void line(std::string id, glm::vec3 start, glm::vec3 end);
        static void line(std::string id, glm::vec3 start, glm::vec3 end, glm::vec3 color);

        static void grid(std::string id, float size);
        static void grid(std::string id, float size, glm::vec3 color);

        static void printJointsTree(Animator::Joint* joint);
        static void printJointsTree(Animator::Joint* joint, std::string prefix);
    private:
        static inline std::map<std::string, Line*> lines = {};
};