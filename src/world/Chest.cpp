#include <opengl.h>

#include <world/Chest.hpp>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>

using namespace world;

// [     8.454 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:20 ] Processing model: assets/models/props/chest.dae
// [     8.464 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:172 ] Armature_root:000001EC1BA0D850
// [     8.464 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:172 ] Armature_top:000001EC1BB20B10
// [     8.464 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:186 ] Armature_root:000001EC1BA0D850
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:186 ] Armature_top:000001EC1BB20B10
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:82 ] Dumping joints:
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:88 ] 0: root (parent: (null), matrix: 1.00 0.00 0.00 0.00 ...)
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:88 ] 1: Armature_root (parent: root, matrix: 1.00 0.00 0.00 0.00 ...)
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:88 ] 2: Armature_top (parent: Armature_root, matrix: 1.00 0.00 0.00 0.00 ...)

Chest::Chest(glm::mat4 model) {
    this->model = model * glm::scale(glm::vec3(0.01f));
}

Chest::~Chest() {
}

void Chest::update() {
}

void Chest::draw(glm::mat4 mat) {
    ResourceLoader *res = resourceLoaderExternal;
    glUseProgram(res->p_color_armature);
    glUniformMatrix4fv(res->p_color_armature_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(this->model));
    glUniformMatrix4fv(res->p_color_armature_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(mat * this->model));
    std::vector<glm::mat4> test(50);

    glUniform1i(resourceLoaderExternal->p_color_armature_uni_caustics, 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniformMatrix4fv(res->p_color_armature_uni_jointTransforms, 50, GL_FALSE, glm::value_ptr(test[0]));
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
        glUniformMatrix4fv(resourceLoaderExternal->p_armature_shadow_uni_jointTransforms, 50, GL_FALSE, glm::value_ptr(test[0]));

        Core::DrawContext(*context);
    }
}