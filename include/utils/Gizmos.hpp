#pragma once

#include <string>
#include <vector>
#include <functional>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Animator/Joint.hpp>

namespace gizmos {

    class Line {
        public:
            Line(glm::vec3 start, glm::vec3 end);
            ~Line();
            int setMVP(glm::mat4 mvp);
            int setColor(glm::vec3 color);
            int setStart(glm::vec3 start);
            int setEnd(glm::vec3 end);
            int draw();
        private:
            int shaderProgram;
            unsigned int VBO, VAO;
            std::vector<float> vertices;
            glm::vec3 startPoint;
            glm::vec3 endPoint;
            glm::mat4 MVP;
            glm::vec3 lineColor;

            GLuint colorUni;
            GLuint MVPUni;
            GLuint startUni;
            GLuint endUni;
    };

    class Gizmos {
        public:
            static void init();
            static void draw();

            static void onDraw(std::function<void()> callback);

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
            static inline std::vector<std::function<void()>> onDrawEvents = {};
    };

}