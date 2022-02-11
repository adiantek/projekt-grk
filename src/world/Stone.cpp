#include <opengl.h>

#include <ResourceLoader.hpp>
#include <world/Stone.hpp>
#include <Water/Water.hpp>

using namespace world;

Stone::Stone(glm::mat4 modelMatrix, int model, float scale) {
    this->modelMatrix = modelMatrix;
    this->model = model;
    physx::PxMat44 mat(glm::value_ptr(modelMatrix));
    physx::PxTransform pose(mat);
    physx::PxBoxGeometry geometry;
    switch (model) {
        case 0:
            geometry = resourceLoaderExternal->m_rocks_rock_big1->createGeometryAABB(glm::vec3(scale));
            break;
        case 1:
            geometry = resourceLoaderExternal->m_rocks_rock_small1->createGeometryAABB(glm::vec3(scale));
            break;
        case 2:
            geometry = resourceLoaderExternal->m_rocks_rock_small2->createGeometryAABB(glm::vec3(scale));
            break;
    }
    this->rigidBody = new physics::RigidBody(true, pose, geometry, this);
}

Stone::~Stone() {
    delete this->rigidBody;
}

void Stone::update() {
}

void Stone::draw(glm::mat4 mat) {
    ResourceLoader *res = resourceLoaderExternal;
    glUseProgram(res->p_rock);
    glUniformMatrix4fv(res->p_rock_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat * this->modelMatrix));
    glUniformMatrix4fv(res->p_rock_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
    glUniform1i(resourceLoaderExternal->p_rock_uni_caustics, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniform1i(resourceLoaderExternal->p_rock_uni_colorTexture, 1);
    glUniform1i(resourceLoaderExternal->p_rock_uni_aoMap, 2);
    glUniform1i(resourceLoaderExternal->p_rock_uni_depthMap, 3);
    glUniform1i(resourceLoaderExternal->p_rock_uni_normalSampler, 4);
    switch (model) {
        case 0:
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_big1_rock_big1_albedo);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_big1_rock_big1_ao);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_big1_rock_big1_height);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_big1_rock_big1_normal);
            Core::DrawContext(*(res->m_rocks_rock_big1->getMeshes()[0]->getRenderContext()));
            break;
        case 1:
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small1_rock_small1_albedo);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small1_rock_small1_ao);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small1_rock_small1_height);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small1_rock_small1_normal);
            Core::DrawContext(*(res->m_rocks_rock_small1->getMeshes()[0]->getRenderContext()));
            break;
        case 2:
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small2_rock_small2_albedo);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small2_rock_small2_ao);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small2_rock_small2_height);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small2_rock_small2_normal);
            Core::DrawContext(*(res->m_rocks_rock_small2->getMeshes()[0]->getRenderContext()));
            break;
    }
}

void Stone::drawShadow(glm::mat4 mat) {
    glm::mat4 viewMatrix = mat;
    glm::mat4 modelMatrix = this->modelMatrix;
    ResourceLoader *res = resourceLoaderExternal;
    std::vector<Mesh *> meshes;
    switch (model) {
        case 0:
            meshes = res->m_rocks_rock_big1->getMeshes();
            break;
        case 1:
            meshes = res->m_rocks_rock_small1->getMeshes();
            break;
        case 2:
            meshes = res->m_rocks_rock_small2->getMeshes();
            break;
    }

    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh *mesh = meshes[i];

        Core::RenderContext *context = mesh->getRenderContext();

        glUseProgram(resourceLoaderExternal->p_environment_map);
        glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        Core::DrawContext(*context);
    }
}