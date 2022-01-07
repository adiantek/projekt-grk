#pragma once

#include <vector>
#include <glm/ext.hpp>

#include <Resources/Material.hpp>
#include <Resources/Model.hpp>
#include <Animator/Joint.hpp>

class GameObject {
   public:
    GameObject::GameObject(std::string name);

    GameObject* setPosition(glm::vec3 position);
    GameObject* setRotation(glm::vec3 rotation);
    GameObject* setScale(glm::vec3 scale);
    GameObject* setMaterials(std::vector<Material*> materials);
    GameObject* setMaterial(Material* material);
    GameObject* setModel(Model* model);

    std::string getName();

    void init();
    void update();
    void draw();

    glm::mat4 getModelMatrix();

   private:
    std::string name;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Model* model;
    std::vector<Material*> materials;
    Animator::Joint* rootJoint;
};