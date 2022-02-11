#include <opengl.h>

#include <world/Chest.hpp>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>
#include <Time/Time.hpp>
#include <Sound.hpp>
#include <Robot/Robot.hpp>

using namespace world;

Chest::Chest(glm::mat4 model, int coins) {
    this->object3DType = 1;
    this->model = model;
    this->coins = coins;
    physx::PxTransform pose(physx::PxMat44(glm::value_ptr(model)));
    physx::PxTriangleMeshGeometry geometry = resourceLoaderExternal->m_props_chest->createGeometry(glm::vec3(0.01f));
    this->rigidBody = new physics::RigidBody(true, pose, geometry, this);
    geometry.triangleMesh->release();

    ResourceLoader *res = resourceLoaderExternal;
    this->coverJoint = res->m_props_chest->getJoint("Armature_top");
}

glm::vec3 Chest::getPosition() {
    return glm::vec3(this->model[3][0], this->model[3][1], this->model[3][2]);
}

void Chest::toggle() {
    if (!this->isOpen && this->coins > 0) {
        while (this->coins--) {
            Coin *coin = new Coin(model * glm::translate(glm::vec3(
                random->nextFloat() * 1.2f - 0.6f,
                random->nextFloat() * 2.0f - 1.0f,
                random->nextFloat() * 1.2f - 0.6f)));
            coin->rigidBody->grabbed = true;
            robot->coins.push_back(coin);
        }
    }
    this->isOpen = !this->isOpen;
    if (this->isOpen) {
        sound->openChest(this->model[3][0], this->model[3][1], this->model[3][2]);
    } else {
        sound->closeChest(this->model[3][0], this->model[3][1], this->model[3][2]);
    }
}

Chest::~Chest() {
    delete this->rigidBody;
}

void Chest::update() {
    float targetAnimation = this->isOpen ? 1.0f : 0.0f;
    if (targetAnimation != openingAnimationStage) {
        openingAnimationStage += 2.5f * timeExternal->deltaTime * (this->isOpen ? 1 : -1);
        if (openingAnimationStage < 0.0f) {
            openingAnimationStage = 0.0f;
        }
        if (openingAnimationStage > 1.0f) {
            openingAnimationStage = 1.0f;
        }

        float rotationStage = glm::smoothstep(0.0f, 1.0f, openingAnimationStage);
        float rotation = -80.0f * rotationStage;
        this->jointTransforms[2] =
            glm::translate(this->coverJoint->getOrigin() * 100.0f)
            * glm::eulerAngleX(glm::radians(rotation))
            * glm::translate(-this->coverJoint->getOrigin() * 100.0f);
    }
}

void Chest::draw(glm::mat4 mat) {
    ResourceLoader *res = resourceLoaderExternal;
    glUseProgram(res->p_color_armature);
    glUniformMatrix4fv(res->p_color_armature_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(this->model));
    glUniformMatrix4fv(res->p_color_armature_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(mat * this->model));

    glUniform1i(resourceLoaderExternal->p_color_armature_uni_caustics, 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniformMatrix4fv(res->p_color_armature_uni_jointTransforms, 50, GL_FALSE, glm::value_ptr(this->jointTransforms[0]));
    glUniform1i(res->p_color_armature_uni_albedoTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_reinforcment_albedo);
    glUniform1i(res->p_color_armature_uni_aoTexture, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_reinforcment_ao);
    glUniform1i(res->p_color_armature_uni_metallicTexture, 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_reinforcment_metallic);
    glUniform1i(res->p_color_armature_uni_normalTexture, 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_reinforcment_normal);
    glUniform1i(res->p_color_armature_uni_roughnessTexture, 4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_reinforcment_roughness);
    Core::DrawContext(*(res->m_props_chest->getMeshes()[0]->getRenderContext()));
    glUniform1i(res->p_color_armature_uni_albedoTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_wood_albedo);
    glUniform1i(res->p_color_armature_uni_aoTexture, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_wood_ao);
    glUniform1i(res->p_color_armature_uni_metallicTexture, 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_wood_metallic);
    glUniform1i(res->p_color_armature_uni_normalTexture, 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_wood_normal);
    glUniform1i(res->p_color_armature_uni_roughnessTexture, 4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_wood_roughness);
    Core::DrawContext(*(res->m_props_chest->getMeshes()[1]->getRenderContext()));
}

void Chest::drawShadow(glm::mat4 mat) {
    glm::mat4 viewMatrix = mat;
    glm::mat4 modelMatrix = this->model;
    ResourceLoader *res = resourceLoaderExternal;
    std::vector<Mesh *> meshes = res->m_props_chest->getMeshes();

    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;
    std::vector<glm::mat4> test(50);


    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];

        Core::RenderContext* context = mesh->getRenderContext();

        glUseProgram(resourceLoaderExternal->p_armature_shadow);

        // Set transformation
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_jointTransforms, 50, GL_FALSE, glm::value_ptr(this->jointTransforms[0]));

        Core::DrawContext(*context);
    }
}