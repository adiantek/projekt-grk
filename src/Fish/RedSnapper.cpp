#include <Fish/RedSnapper.hpp>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>
#include <Time/Time.hpp>

#define REDSNAPPER_SCALE 0.5f

using namespace fish;

RedSnapper::RedSnapper(glm::vec3 position, world::World* world)
: Fish(resourceLoaderExternal->m_entities_red_snapper, position, world, glm::vec3(REDSNAPPER_SCALE)) {}

RedSnapper::~RedSnapper() {}

void RedSnapper::update() {
    Fish::update();
    // I should be flying forward
    this->rigidBody->rotateForward(this->rotation);
}

void RedSnapper::draw(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(REDSNAPPER_SCALE));
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_normal_fish);
    glUniform1f(resourceLoaderExternal->p_normal_fish_uni_time, (float)timeExternal->lastFrame);
    glUniform1i(resourceLoaderExternal->p_normal_fish_uni_colorTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_entities_red_snapper_red_snapper_albedo);
    glUniform1i(resourceLoaderExternal->p_normal_fish_uni_caustics, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniform1i(resourceLoaderExternal->p_normal_fish_uni_normalTexture, 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_entities_red_snapper_red_snapper_normal);
    glUniformMatrix4fv(resourceLoaderExternal->p_normal_fish_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_normal_fish_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}

void RedSnapper::drawShadow(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(REDSNAPPER_SCALE));
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_fish_shadow);
    glUniform1f(resourceLoaderExternal->p_fish_shadow_uni_time, (float)timeExternal->lastFrame);
    glUniformMatrix4fv(resourceLoaderExternal->p_fish_shadow_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_fish_shadow_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}

glm::vec3 RedSnapper::findTarget() {
    glm::vec3 offset = glm::vec3(this->random->nextFloat(-40.0f, 40.0f), 0.0f, this->random->nextFloat(-40.0f, 40.0f)) + robot->position;
    world::Chunk* chunk = this->world->getChunkAt(world::ChunkPosition(offset));
    if (chunk) {
        offset.y = (waterObject->getY() - chunk->maxY) / 1.5f + chunk->maxY;
    }
    return glm::vec3(offset);
}