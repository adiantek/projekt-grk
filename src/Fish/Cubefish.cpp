#include <Fish/Cubefish.hpp>
#include <Water/Water.hpp>
#include <Render_Utils.h>
#include <ResourceLoader.hpp>

using namespace fish;

Cubefish::Cubefish(glm::vec3 position, float mass, float density) {
    physx::PxTransform initPose = physx::PxTransform(position.x, position.y, position.z);
	physx::PxBoxGeometry geometry = resourceLoaderExternal->m_primitives_cube->createGeometryAABB();
    this->rigidBody = new physics::RigidBody(false, initPose, geometry, this, 0.5f, 0.5f, 0.001f);
    this->rigidBody->setMass(mass);
	this->rigidBody->density = density;
    this->model = resourceLoaderExternal->m_primitives_cube;
}

Cubefish::Cubefish(glm::mat4 modelMatrix, float mass, float density, Model* model, bool dynamic) {
    physx::PxMat44 pose(glm::value_ptr(modelMatrix));
    physx::PxTransform initPose = physx::PxTransform(pose);
	physx::PxBoxGeometry geometry = model->createGeometryAABB();
    this->rigidBody = new physics::RigidBody(!dynamic, initPose, geometry, this, 0.5f, 0.5f, 0.001f);
    if (dynamic) {
        this->rigidBody->setMass(mass);
	    this->rigidBody->density = density;
    }
    this->model = model;
}

Cubefish::~Cubefish() {
    delete this->rigidBody;
}

void Cubefish::update() {}

void Cubefish::draw(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix();
    glm::mat4 transformation = mat * model;
    glEnable(GL_CULL_FACE);
    glUseProgram(resourceLoaderExternal->p_cubefish);
    glUniform1i(resourceLoaderExternal->p_cubefish_uni_colorTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall);
    glUniform1i(resourceLoaderExternal->p_cubefish_uni_normalSampler, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_normal);
    glUniform1i(resourceLoaderExternal->p_cubefish_uni_caustics, 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    glUniform1i(resourceLoaderExternal->p_cubefish_uni_depthMap, 3);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_height);
    glUniform1i(resourceLoaderExternal->p_cubefish_uni_roughnessMap, 4);
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_roughness);
    glUniform1i(resourceLoaderExternal->p_cubefish_uni_aoMap, 5);
    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_ao);
    glUniformMatrix4fv(resourceLoaderExternal->p_cubefish_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_cubefish_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    glUniformMatrix4fv(resourceLoaderExternal->p_cubefish_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
    glDisable(GL_CULL_FACE);
}

void Cubefish::drawShadow(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix();
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}