#include <Fish/Fish.hpp>
#include <Physics/Physics.hpp>
#include <ResourceLoader.hpp>
#include <Water/Water.hpp>
#include <Robot/Robot.hpp>
#include <Time/Time.hpp>

using namespace fish;

Fish::Fish(Model* model, glm::vec3 position, world::World* world, glm::vec3 scale) {
    this->model = model;
    physx::PxTransform pose = physx::PxTransform(position.x, position.y, position.z);
    physx::PxBoxGeometry geometry = model->createGeometryAABB(scale);
    this->rigidBody = new physics::RigidBody(false, pose, geometry, this);

    this->rigidBody ->setMass(1.0f);
    this->rigidBody->density = 0.997f;
    this->rigidBody->drag = false;
    this->world = world;
    this->random = new Random((long long)this);
}

Fish::~Fish() {
    delete this->rigidBody;
    delete this->random;
}

void Fish::draw(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix();
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_shader_color);
    glUniform3f(resourceLoaderExternal->p_shader_color_uni_objectColor, 1.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(resourceLoaderExternal->p_shader_color_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_shader_color_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}

void Fish::drawShadow(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix();
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    for (auto mesh : this->model->getMeshes()) {
        Core::DrawContext(*mesh->getRenderContext());
    }
}

void Fish::update() {
    glm::vec3 position = glm::vec3(this->rigidBody->getModelMatrix()[3]);
    world::Chunk* chunk = this->world->getChunkAt(world::ChunkPosition(position));
    float waterDistance = position.y - waterObject->getY() - 2.0f;
    if (waterDistance < 0.0f && waterDistance > -10.0f) { // I should be underwater
        this->rigidBody->addForce(glm::vec3(0.0f, std::max(10.0f / waterDistance, -15.0f), 0.0f));
    } // Else I am over the water level and let gravity do the thing
    if (chunk) { // I should be higher than ground
        float chunkDistance = abs(position.y - chunk->maxY);
        if (chunkDistance < 1.0f)
            this->rigidBody->addForce(glm::vec3(0.0f, 1.5f, 0.0f));
    }
    // I want to go to the target
    glm::vec3 targetDirection = this->target - position;
    this->rigidBody->addForce(targetDirection * 0.00002f * glm::length(targetDirection));
    // I dont like robots
    glm::vec3 robotDirection = robot->position - position;
    if (glm::length(robotDirection) < 10.0f) {
        this->rigidBody->addForce(-robotDirection * (1.0f / glm::length(targetDirection)));
    }
}

void Fish::setTarget(glm::vec3 target) {
    this->target = target;
}