#include <utils/Cube.hpp>
#include <opengl.h>
#include <ResourceLoader.hpp>

using namespace utils;

GLuint Cube::vao = 0;

void Cube::init() {
    if (Cube::initialized) {
        return;
    }
    Cube::initialized = true;
    vertex::VertexBuffer vbo(&vertex::POS, 32);
    
    vbo.pos(-1.f, -1.f, -1.f)->end();
    vbo.pos(1.f, -1.f, -1.f)->end();

    vbo.pos(1.f, -1.f, -1.f)->end();
    vbo.pos(1.f, -1.f, 1.f)->end();

    vbo.pos(1.f, -1.f, 1.f)->end();
    vbo.pos(-1.f, -1.f, 1.f)->end();
    
    vbo.pos(-1.f, -1.f, 1.f)->end();
    vbo.pos(-1.f, -1.f, -1.f)->end();

    vbo.pos(-1.f, 1.f, -1.f)->end();
    vbo.pos(1.f, 1.f, -1.f)->end();

    vbo.pos(1.f, 1.f, -1.f)->end();
    vbo.pos(1.f, 1.f, 1.f)->end();

    vbo.pos(1.f, 1.f, 1.f)->end();
    vbo.pos(-1.f, 1.f, 1.f)->end();
    
    vbo.pos(-1.f, 1.f, 1.f)->end();
    vbo.pos(-1.f, 1.f, -1.f)->end();

    vbo.pos(-1.f, 1.f, -1.f)->end();
    vbo.pos(-1.f, -1.f, -1.f)->end();

    vbo.pos(1.f, 1.f, -1.f)->end();
    vbo.pos(1.f, -1.f, -1.f)->end();

    vbo.pos(1.f, 1.f, 1.f)->end();
    vbo.pos(1.f, -1.f, 1.f)->end();

    vbo.pos(-1.f, 1.f, 1.f)->end();
    vbo.pos(-1.f, -1.f, 1.f)->end();

    vbo.pos(-1.f, -1.f, -1.f)->end();
    vbo.pos(1.f, 1.f, 1.f)->end();

    vbo.pos(1.f, -1.f, 1.f)->end();
    vbo.pos(-1.f, 1.f, -1.f)->end();

    vbo.pos(-1.f, -1.f, 1.f)->end();
    vbo.pos(1.f, 1.f, -1.f)->end();

    vbo.pos(1.f, -1.f, -1.f)->end();
    vbo.pos(-1.f, 1.f, 1.f)->end();


    glGenVertexArrays(1, &Cube::vao);
    glBindVertexArray(Cube::vao);
    vbo.uploadVBO();
    vbo.configurePos(0);
}

void Cube::draw(glm::mat4 mat, glm::vec3 position, glm::vec3 size, glm::vec3 color, bool diagonals) {
    Cube::init();
    glm::mat4 model = mat * glm::translate(position) * glm::scale(size);
    glUseProgram(resourceLoaderExternal->p_simple_uni_color_shader);
    glBindVertexArray(Cube::vao);
    glUniform3fv(resourceLoaderExternal->p_simple_uni_color_shader_uni_color, 1, glm::value_ptr(color));
    glUniformMatrix4fv(resourceLoaderExternal->p_simple_uni_color_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_LINES, 0, diagonals ? 32 : 24);
}