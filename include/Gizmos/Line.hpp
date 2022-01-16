#pragma once

#include <vector>
#include <glm/ext.hpp>

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