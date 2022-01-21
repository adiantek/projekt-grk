#include <utils/Line.hpp>
#include <opengl.h>
#include <ResourceLoader.hpp>

using namespace utils;

GLuint Line::vao = 0;

void Line::init() {
    if (Line::initialized) {
        return;
    }
    Line::initialized = true;
    vertex::VertexBuffer vbo(&vertex::POS, 2);
    
    vbo.pos(0.f, 0.f, 0.f)->end();
    vbo.pos(1.f, 0.f, 0.f)->end();

    glGenVertexArrays(1, &Line::vao);
    glBindVertexArray(Line::vao);
    vbo.uploadVBO();
    vbo.configurePos(0);
}

void Line::draw(glm::mat4 mat, glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    Line::init();

    glm::mat4 rotationMatrix = glm::orientation(glm::normalize(end - start), glm::vec3(1, 0, 0));
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(glm::distance(start, end)));

    mat = mat * glm::translate(start) * rotationMatrix * scaleMatrix;

    glUseProgram(resourceLoaderExternal->p_simple_uni_color_shader);
    glBindVertexArray(Line::vao);
    glUniform3fv(resourceLoaderExternal->p_simple_uni_color_shader_uni_color, 1, glm::value_ptr(color));
    glUniformMatrix4fv(resourceLoaderExternal->p_simple_uni_color_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glDrawArrays(GL_LINES, 0, 2);
}