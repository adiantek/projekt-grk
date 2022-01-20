#include <opengl.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Animator/Joint.hpp>
#include <Camera/Camera.hpp>
#include <utils/Gizmos.hpp>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>

using namespace gizmos;

void Gizmos::init() {}

void Gizmos::draw() {
    glDisable(GL_DEPTH_TEST);
    for (auto callback : Gizmos::onDrawEvents) {
        callback();
    }
}

void Gizmos::onDraw(std::function<void()> callback) {
    Gizmos::onDrawEvents.push_back(callback);
}

void Gizmos::cube(std::string id, glm::vec3 position) {
    glm::vec3 size = glm::vec3(0.05f);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(id, position, size, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, glm::vec3 size) {
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(id, position, size, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, float size) {
    glm::vec3 sizeVector = glm::vec3(size);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(id, position, sizeVector, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, float size, glm::vec3 color) {
    glm::vec3 sizeVector = glm::vec3(size);

    return Gizmos::cube(id, position, sizeVector, color);
}

void Gizmos::cube(std::string id, glm::vec3 position, glm::vec3 size, glm::vec3 color) {
    Gizmos::line(id + "_1", position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_2", position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_3", position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_4", position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), color);

    Gizmos::line(id + "_5", position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_6", position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_7", position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_8", position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color);

    Gizmos::line(id + "_9", position + glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_10", position + glm::vec3(-size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2), color);
    Gizmos::line(id + "_11", position + glm::vec3(size.x / 2, -size.y / 2, -size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2), color);
    Gizmos::line(id + "_12", position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2), position + glm::vec3(size.x / 2, size.y / 2, size.z / 2), color);
}

void Gizmos::line(std::string id, glm::vec3 start, glm::vec3 end) {
    return Gizmos::line(id, start, end, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Gizmos::line(std::string id, glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    std::map<std::string, Line*>::iterator iter = Gizmos::lines.find(id);
    Line* line;

    if (iter != Gizmos::lines.end()) {
        line = iter->second;
        line->setStart(start);
        line->setEnd(end);
        line->setColor(color);
    } else {
        line = new Line(start, end);
        line->setColor(color);

        Gizmos::lines[id] = line;
    }

    line->setMVP(camera->getTransformationMatrix());
    line->draw();
}

void Gizmos::grid(std::string id, float size) {
}

void Gizmos::grid(std::string id, float size, glm::vec3 color) {
}

void Gizmos::printJointsTree(Animator::Joint* joint) {
    Gizmos::printJointsTree(joint, "");
}

void Gizmos::printJointsTree(Animator::Joint* joint, std::string prefix) {
    std::cout << prefix << "(" << joint->index << ") " << joint->name << std::endl;

    for (Animator::Joint* child : joint->children) {
        Gizmos::printJointsTree(child, prefix + "  ");
    }
}

Line::Line(glm::vec3 start, glm::vec3 end) {

    lineColor = glm::vec3(1,1,1);
    MVP = glm::mat4(1.0f);

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 MVP;\n"
        "uniform vec3 startPos;\n"
        "uniform vec3 endPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = MVP * vec4((1 - aPos.x) * startPos.x + (aPos.x * endPos.x), (1 - aPos.y) * startPos.y + (aPos.y * endPos.y), (1 - aPos.z) * startPos.z + (aPos.z * endPos.z), 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\n\0";

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    vertices = {
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    colorUni = glGetUniformLocation(shaderProgram, "color");
    MVPUni = glGetUniformLocation(shaderProgram, "MVP");
    startUni = glGetUniformLocation(shaderProgram, "startPos");
    endUni = glGetUniformLocation(shaderProgram, "endPos");

    this->setStart(start);
    this->setEnd(end);
}

int Line::setMVP(glm::mat4 mvp) {
    MVP = mvp;
    return 1;
}

int Line::setColor(glm::vec3 color) {
    lineColor = color;
    return 1;
}

int Line::draw() {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(this->MVPUni, 1, GL_FALSE, &MVP[0][0]);
    glUniform3fv(this->colorUni, 1, &lineColor[0]);
    glUniform3fv(this->startUni, 1, &startPoint[0]);
    glUniform3fv(this->endUni, 1, &endPoint[0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    return 1;
}

int Line::setStart(glm::vec3 start) {
    startPoint = start;
    return 1;
}

int Line::setEnd(glm::vec3 end) {
    endPoint = end;
    return 1;
}

Line::~Line() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}