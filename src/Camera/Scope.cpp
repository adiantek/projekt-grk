#include <Camera/Scope.hpp>
#include <Camera/Camera.hpp>
#include <Controller/Controller.hpp>
#include <utils/Line.hpp>

Scope::Scope(float size, glm::vec3 color) {
    scope = this;
    this->size = size;
    this->color = color;
}

Scope::~Scope() {}

void Scope::draw() {
    glDisable(GL_DEPTH_TEST);
    if (controller->mouseRightClicked) {
        float ratio = (float) camera->width / (float) camera->height;
        if (controller->scopeMode == 0) {
            utils::Line::draw(glm::mat4(), glm::vec3(0.0f, -this->size * ratio, -1.0f), glm::vec3(0.0f, this->size * ratio, -1.0f), this->color);
            utils::Line::draw(glm::mat4(), glm::vec3(-this->size, 0.0f, -1.0f), glm::vec3(this->size, 0.0f, -1.0f), this->color);
        } else if (controller->scopeMode == 1) {
            utils::Line::draw(glm::mat4(), glm::vec3(-this->size, -this->size * ratio, -1.0f), glm::vec3(this->size, this->size * ratio, -1.0f), this->color);
            utils::Line::draw(glm::mat4(), glm::vec3(this->size, -this->size * ratio, -1.0f), glm::vec3(-this->size, this->size * ratio, -1.0f), this->color);
        }
    }
    glEnable(GL_DEPTH_TEST);
}

Scope* scope;