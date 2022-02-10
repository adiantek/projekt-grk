#include <world/Coin.hpp>
#include <opengl.h>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>
#include <Time/Time.hpp>


using namespace world;

Coin::Coin(glm::mat4 model) {
    this->model = model * glm::scale(glm::vec3(0.5f));
    physx::PxTransform pose(physx::PxMat44(glm::value_ptr(model)));
    physx::PxBoxGeometry geometry = resourceLoaderExternal->m_props_coin->createGeometryAABB(glm::vec3(0.5f));
    this->rigidBody = new physics::RigidBody(false, pose, geometry, this);
    this->rigidBody->density = 2.0f;
    this->rigidBody->setMass(3.0f);
}

Coin::~Coin() {
    delete this->rigidBody;
}

void Coin::update() {}

void Coin::draw(glm::mat4 mat) {
    ResourceLoader *res = resourceLoaderExternal;
    glm::mat4 model = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(0.5f));
    glUseProgram(res->p_chunk);
    glUniformMatrix4fv(res->p_chunk_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(res->p_chunk_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat * model));

    glUniform1i(resourceLoaderExternal->p_chunk_uni_caustics, 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniform1i(res->p_chunk_uni_colorTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_coin_coin_albedo);
    glUniform1i(res->p_chunk_uni_aoMap, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_coin_coin_ao);
    // glUniform1i(res->p_color_armature_uni_metallicTexture, 2);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, res->tex_props_coin_coin_metallic);
    glUniform1i(res->p_chunk_uni_normalSampler, 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_coin_coin_normal);
    glUniform1i(res->p_chunk_uni_roughnessMap, 4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_coin_coin_roughness);
    glUniform1i(res->p_chunk_uni_depthMap, 6);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_coin_coin_height);
    for (auto mesh : res->m_props_coin->getMeshes())
        Core::DrawContext(*(mesh->getRenderContext()));
}

void Coin::drawShadow(glm::mat4 mat) {
    glm::mat4 viewMatrix = mat;
    glm::mat4 modelMatrix = this->rigidBody->getModelMatrix() * glm::scale(glm::vec3(0.5f));
    ResourceLoader *res = resourceLoaderExternal;
    std::vector<Mesh *> meshes = res->m_props_coin->getMeshes();

    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;


    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];

        Core::RenderContext* context = mesh->getRenderContext();

        glUseProgram(resourceLoaderExternal->p_environment_map);
        glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        Core::DrawContext(*context);
    }
}