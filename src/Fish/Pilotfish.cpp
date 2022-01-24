#include <Fish/Pilotfish.hpp>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>

Pilotfish::Pilotfish(glm::vec3 position, world::World* world)
: Fish(resourceLoaderExternal->m_pilotfish, position, world) {}

Pilotfish::~Pilotfish() {}

void Pilotfish::update() {
    Fish::update();
    // I should be flying forward
    this->rigidBody->rotateForward(glm::eulerAngleY(glm::radians(90.0f)));
}

void Pilotfish::draw(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(0.3f));
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_pilotfish);
    glUniform1i(resourceLoaderExternal->p_pilotfish_uni_colorTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_pilotfish);
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
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(0.3f));
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}