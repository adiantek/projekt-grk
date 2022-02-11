#include <opengl.h>

#include <ResourceLoader.hpp>
#include <world/Stone.hpp>

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
    glUseProgram(res->p_simple_tex_shader);
    glUniformMatrix4fv(res->p_simple_tex_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat * this->modelMatrix));
    glUniform1i(res->p_simple_tex_shader_uni_textureSampler, 0);
    glActiveTexture(GL_TEXTURE0);
    switch (model) {
        case 0:
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_big1_rock_big1_albedo);
            Core::DrawContext(*(res->m_rocks_rock_big1->getMeshes()[0]->getRenderContext()));
            break;
        case 1:
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small1_rock_small1_albedo);
            Core::DrawContext(*(res->m_rocks_rock_small1->getMeshes()[0]->getRenderContext()));
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, res->tex_rocks_rock_small2_rock_small2_albedo);
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