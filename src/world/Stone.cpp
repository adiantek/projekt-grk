#include <opengl.h>

#include <ResourceLoader.hpp>
#include <world/Stone.hpp>

using namespace world;

Stone::Stone(glm::mat4 modelMatrix, int model) {
    this->modelMatrix = modelMatrix;
    this->model = model;
}

Stone::~Stone() {
}

void Stone::update() {
}

void Stone::draw(glm::mat4 mat) {
    ResourceLoader *res = resourceLoaderExternal;
    glUseProgram(res->p_simple_tex_shader);
    glUniformMatrix4fv(res->p_simple_tex_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat * this->modelMatrix));
    glUniform1i(res->p_simple_tex_shader_uni_textureSampler, 0);
    glActiveTexture(GL_TEXTURE0);
    switch (model) {
        case 0:
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_big1_rock_big1_albedo);
            Core::DrawContext(*(res->m_rocks_rock_big1->getMeshes()[0]->getRenderContext()));
            break;
        case 1:
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small1_rock_small1_albedo);
            Core::DrawContext(*(res->m_rocks_rock_small1->getMeshes()[0]->getRenderContext()));
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small2_rock_small2_albedo);
            Core::DrawContext(*(res->m_rocks_rock_small2->getMeshes()[0]->getRenderContext()));
            break;
    }
}

void Stone::drawShadow(glm::mat4 mat) {
}