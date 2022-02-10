#include <vector>
#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Resources/GameObject.hpp>
#include <Resources/Resources.hpp>
#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>

GameObject::GameObject(std::string name) {
    this->name = name;

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

GameObject::~GameObject() {
    // TODO?
}

GameObject* GameObject::setPosition(glm::vec3 position) {
    this->position = position;
    return this;
}

GameObject* GameObject::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
    return this;
}

GameObject* GameObject::setScale(glm::vec3 scale) {
    this->scale = scale;
    return this;
}

GameObject* GameObject::setMaterials(std::vector<Material *> materials) {
    this->materials = materials;
    return this;
}

GameObject* GameObject::setMaterial(Material *material) {
    this->materials = { material };
    return this;
}

GameObject* GameObject::setModel(Model* model) {
    this->model = model;
    return this;
}

glm::mat4 GameObject::getModelMatrix() {
    return 
        glm::translate(this->position)
        * glm::eulerAngleYXZ(glm::radians(this->rotation.y), glm::radians(this->rotation.x), glm::radians(this->rotation.z))
        * glm::rotate(glm::radians(-90.0f), glm::vec3(1,0,0))
        * glm::scale(this->scale);
}

void GameObject::update() {
}

void GameObject::draw(glm::mat4 mat) {
    glm::mat4 viewMatrix = mat;
    glm::mat4 modelMatrix = this->getModelMatrix();
    std::vector<Mesh *> meshes = this->model->getMeshes();

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];
        Material* material = this->materials[i];

        Core::RenderContext* context = mesh->getRenderContext();

        if (material == nullptr) {
            material = Resources::MATERIALS.DEFAULT;
        }
        
        material->use();

        material
            ->setModelMatrix(modelMatrix)
            ->setModelViewProjectionMatrix(modelViewProjectionMatrix);

        if (this->model->hasJoints()) {
            material->setJointTransforms(this->getJointTransforms());
        }
        

        Core::DrawContext(*context);
    }
}

void GameObject::drawShadow(glm::mat4 mat) {
    glm::mat4 viewMatrix = mat;
    glm::mat4 modelMatrix = this->getModelMatrix();
    std::vector<Mesh *> meshes = this->model->getMeshes();

    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];

        Core::RenderContext* context = mesh->getRenderContext();

        glUseProgram(resourceLoaderExternal->p_armature_shadow);

        // Set transformation
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_jointTransforms, 50, GL_FALSE, glm::value_ptr(this->getJointTransforms()[0]));

        Core::DrawContext(*context);
    }
}

Model* GameObject::getModel() {
    return this->model;
}

std::vector<glm::mat4> GameObject::getJointTransforms() {
    std::vector<glm::mat4> jointsArray = {};
    std::vector<Animator::Joint*> joints = this->model->getJoints();

    if (joints.size() > 0) {
        for (int i = 0; i < std::min((int)joints.size(), 50); i++) {
            jointsArray.push_back(joints[i]->getTransform());
        }
        for (int i = (int)joints.size(); i < 50; i++) {
            jointsArray.push_back(glm::mat4(1.0f));
        }
    }

    return jointsArray;
}

glm::vec3 GameObject::getScale() {
    return this->scale;
} 