#pragma once

#include <vector>
#include <glm/ext.hpp>

#include <Resources/Material.hpp>
#include <Resources/Model.hpp>
#include <Animator/Joint.hpp>
#include <world/Object3D.hpp>

class GameObject : world::Object3D {
   public:
    GameObject(std::string name);
    virtual ~GameObject();

    GameObject* setPosition(glm::vec3 position);
    GameObject* setRotation(glm::vec3 rotation);
    GameObject* setScale(glm::vec3 scale);
    GameObject* setMaterials(std::vector<Material*> materials);
    GameObject* setMaterial(Material* material);
    GameObject* setModel(Model* model);
    glm::vec3 getScale();
    std::string getName();

    void init();
    void update() override;
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;

    glm::mat4 getModelMatrix();
    std::vector<glm::mat4> getJointTransforms();
    Model* getModel();
    void addJointsToArray(Animator::Joint* headJoint, std::vector<glm::mat4>* jointMatrices);

   private:
    std::string name;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Model* model;
    std::vector<Material*> materials;
};