#pragma once

#include <vector>
#include <glm/ext.hpp>

#include <Resources/Material.hpp>
#include <Resources/Model.hpp>

class GameObject {
   public:
    GameObject::GameObject(std::string name);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    GameObject* setMaterials(std::vector<Material*> materials);
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
    Model* model;
    std::vector<Material*> materials;
};