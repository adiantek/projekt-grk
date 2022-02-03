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
    if (controller->mouseRightClicked) {
        float ratio = (float) camera->width / (float) camera->height;
        utils::Line::draw(glm::mat4(), glm::vec3(-this->size, -this->size * ratio, -1.0f), glm::vec3(this->size, this->size * ratio, -1.0f), this->color);
        utils::Line::draw(glm::mat4(), glm::vec3(this->size, -this->size * ratio, -1.0f), glm::vec3(-size, this->size * ratio, -1.0f), this->color);
    }
}

Scope* scope;