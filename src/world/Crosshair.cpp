#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Robot/Robot.hpp>
#include <vertex/VertexBuffer.hpp>
#include <world/Crosshair.hpp>

using namespace world;

Crosshair::Crosshair() {
    vertex::VertexBuffer vbo(&vertex::POS_COLOR, 6);

    const float size = 0.05f;

    vbo.pos(0.0f, 0.0f, 0.0f)->color(1.0f, 0.0f, 0.0f, 1.0f)->end();
    vbo.pos(size, 0.0f, 0.0f)->color(1.0f, 0.0f, 0.0f, 1.0f)->end();

    vbo.pos(0.0f, 0.0f, 0.0f)->color(0.0f, 1.0f, 0.0f, 1.0f)->end();
    vbo.pos(0.0f, size, 0.0f)->color(0.0f, 1.0f, 0.0f, 1.0f)->end();

    vbo.pos(0.0f, 0.0f, 0.0f)->color(0.5f, 0.5f, 1.0f, 1.0f)->end();
    vbo.pos(0.0f, 0.0f, size)->color(0.5f, 0.5f, 1.0f, 1.0f)->end();

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    this->vbo = vbo.uploadVBO();
    vbo.configureColor(resourceLoaderExternal->p_simple_color_shader_attr_vertexColor);
    vbo.configurePos(resourceLoaderExternal->p_simple_color_shader_attr_vertexPosition);
}

Crosshair::~Crosshair() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Crosshair::update() {
}

void Crosshair::draw(glm::mat4 mat) {
    mat = mat * glm::translate(robot->position);

    glDisable(GL_DEPTH_TEST);
    glUseProgram(resourceLoaderExternal->p_simple_color_shader);
    glUniformMatrix4fv(resourceLoaderExternal->p_simple_color_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glBindVertexArray(this->vao);
    glLineWidth(2.0F);
    glDrawArrays(GL_LINES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}