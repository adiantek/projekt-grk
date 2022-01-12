#include <vector>
#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Resources/GameObject.hpp>
#include <Resources/Resources.hpp>
#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Resources/GlobalEvents.hpp>
#include <Gizmos/Gizmos.hpp>

GameObject::GameObject(std::string name) {
    this->name = name;

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

    GlobalEvents::on(GlobalEvents::ON_GIZMOS_RENDER, [this]() {
        std::vector<Animator::Joint*> joints = this->model->getJoints();
        // Gizmos::joint(this->position, joints[0]);
        for (Animator::Joint* joint : joints) {
            Gizmos::joint(this->position, joint);
        }
    });
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
        * glm::eulerAngleXYZ(glm::radians(this->rotation.x), glm::radians(this->rotation.y), glm::radians(this->rotation.z))
        * glm::rotate(glm::radians(-90.0f), glm::vec3(1,0,0))
        * glm::scale(this->scale);
}

void GameObject::draw() {
    glm::mat4 viewMatrix = camera->getTransformationMatrix();
    glm::mat4 modelMatrix = this->getModelMatrix();
    std::vector<Mesh *> meshes = this->model->getMeshes();

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];
        Material* material = this->materials[i];

        Core::RenderContext* context = this->model->getMeshes()[i]->getRenderContext();

        if (material == nullptr) {
            material = Resources::MATERIALS.DEFAULT;
        }

        glUseProgram(*material->program);
        material->loadToProgram(material->program);

        // Set light
        glUniform3f(glGetUniformLocation(*material->program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

        // Set transformation
        glUniformMatrix4fv(glGetUniformLocation(*material->program, "modelMatrix"), 1, GL_FALSE, (float*)&(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(*material->program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&(modelViewProjectionMatrix));

        if (mesh->hasJoints()) {
            glUniformMatrix4fv(glGetUniformLocation(*material->program, "jointTransforms"), 50, GL_FALSE, (float*)&(this->getJointTransforms(i)[0]));
        }

        Core::DrawContext(*context);
    }
}

std::vector<glm::mat4> GameObject::getJointTransforms(int meshIndex) {
    Mesh* mesh = this->model->getMeshes()[meshIndex];
    std::vector<glm::mat4> jointsArray = {};

    for (int i = 0; i < 50; i++) {
        jointsArray.push_back(glm::mat4());
    }

    // if (!mesh->hasJoints()) return jointsArray;

    // for (int i = 0; i < std::min(mesh->getJointsCount(), 50); i++) {
    //     if (mesh->getJoints()[i]->name.find(std::string("Leg")) != std::string::npos) {
    //         jointsArray[i] = mesh->getJoints()[i]->getTransform()/* * glm::eulerAngleY(glm::radians(90.0f))*/;
    //     } else {
    //         jointsArray[i] = mesh->getJoints()[i]->getTransform();
    //     }
    // }

    return jointsArray;
}

// Not used
void GameObject::addJointsToArray(Animator::Joint* headJoint, std::vector<glm::mat4>* jointMatrices) {
    jointMatrices->push_back(headJoint->getTransform());

    for (Animator::Joint* child : headJoint->children) {
        this->addJointsToArray(child, jointMatrices);
    }
}