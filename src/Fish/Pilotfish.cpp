#include <Fish/Pilotfish.hpp>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>
#include <Time/Time.hpp>

#define PILOTFISH_SCALE 0.2f

Pilotfish::Pilotfish(glm::vec3 position, world::World* world)
: Fish(resourceLoaderExternal->m_entities_pilotfish, position, world, glm::vec3(PILOTFISH_SCALE)) {}

Pilotfish::~Pilotfish() {}

void Pilotfish::update() {
    Fish::update();
    // I should be flying forward
    this->rigidBody->rotateForward(glm::eulerAngleX(glm::radians(-90.0f)));
}

void Pilotfish::draw(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(PILOTFISH_SCALE));
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_pilotfish);
    glUniform1f(resourceLoaderExternal->p_pilotfish_uni_time, (float)timeExternal->lastFrame);
    glUniform1i(resourceLoaderExternal->p_pilotfish_uni_colorTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_entities_pilotfish_pilotfish_albedo);
    glUniform1i(resourceLoaderExternal->p_pilotfish_uni_caustics, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniformMatrix4fv(resourceLoaderExternal->p_pilotfish_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_pilotfish_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}

void Pilotfish::drawShadow(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(PILOTFISH_SCALE));
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}