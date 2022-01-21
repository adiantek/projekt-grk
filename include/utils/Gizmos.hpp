#pragma once

#include <Render_Utils.h>

#include <Animator/Joint.hpp>
#include <functional>
#include <glm/ext.hpp>
#include <string>
#include <vector>

namespace utils {

class Gizmos {
   public:
    static void init();
    static void draw();

    static void onDraw(std::function<void()> callback);

    static void cube(glm::vec3 position, glm::vec3 size, glm::vec3 color);
    static void cube(glm::vec3 position, float size, glm::vec3 color);
    static void cube(glm::vec3 position, glm::vec3 size);
    static void cube(glm::vec3 position, float size);
    static void cube(glm::vec3 position);

    static void line(glm::vec3 start, glm::vec3 end);
    static void line(glm::vec3 start, glm::vec3 end, glm::vec3 color);

    static void grid(std::string id, float size);
    static void grid(std::string id, float size, glm::vec3 color);

    static void printJointsTree(Animator::Joint* joint);
    static void printJointsTree(Animator::Joint* joint, std::string prefix);

    static void dumpJoints(std::vector<Animator::Joint *> joints);

   private:
    static inline std::vector<std::function<void()>> onDrawEvents = {};
};

}  // namespace gizmos