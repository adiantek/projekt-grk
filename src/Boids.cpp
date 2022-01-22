#include <Boids.hpp>
#include <ResourceLoader.hpp>
#include <Random.hpp>
#include <Robot/Robot.hpp>
#include <Water/Water.hpp>
#include <world/ChunkPosition.hpp>

TempFish::TempFish(Core::RenderContext context, glm::vec3 initPos) {
    this->context = context;
    Random x2((long long) initPos.x);
    physx::PxSphereGeometry geometry = physx::PxSphereGeometry(1.0f);
    physx::PxTransform pose = physx::PxTransform(initPos.x, initPos.y, initPos.z);
    this->rigidBody = new physics::RigidBody(false, pose, geometry, this);
    this->rigidBody->setMass(1.0f);
    this->rigidBody->density = 0.997f;
    this->rigidBody->setLinearVelocity(glm::vec3(x2.nextFloat(-2.0f, 2.0f), x2.nextFloat(-0.3f, 0.3f), x2.nextFloat(-2.0f, 2.0f)));
    this->rigidBody->drag = false;
}

TempFish::~TempFish() { 
    delete this->rigidBody;
}

void TempFish::draw(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix();
    glm::mat4 transormation = mat * model;
    glUseProgram(resourceLoaderExternal->p_shader_color);
    glUniform3f(resourceLoaderExternal->p_shader_color_uni_objectColor, 1.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(resourceLoaderExternal->p_shader_color_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_shader_color_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(transormation));
    Core::DrawContext(this->context);
    glUseProgram(0);
}

void TempFish::drawShadow(glm::mat4 mat) {
    glm::mat4 model = this->rigidBody->getModelMatrix();
    glm::mat4 transformation = mat * model;
    glUseProgram(resourceLoaderExternal->p_environment_map);
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(resourceLoaderExternal->p_environment_map_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
    Core::DrawContext(this->context);
    glUseProgram(0);
}


void TempFish::update() {
    glm::vec3 position = glm::vec3(this->rigidBody->getModelMatrix()[3]);
    world::Chunk* chunk = this->world->getChunkAt(world::ChunkPosition(position));
    float waterDistance = position.y - waterObject->getY();
    if (waterDistance < 0.0f) {
        // I should be underwater
        this->rigidBody->addForce(glm::vec3(0.0f, std::max(2.0f / waterDistance, -10.0f), 0.0f));
    } // Else I am over the water level and let gravity do the thing
    if (chunk) {
        // I should be higher than ground
        this->rigidBody->addForce(glm::vec3(0.0f, std::min(2.0f / abs(position.y - chunk->maxY), 10.0f), 0.0f));
    }
}

Boids::Boids(unsigned int amount, Core::RenderContext context, glm::vec3 position, world::World* w) : random(0L) {
    Random x(0);
    for (unsigned int i = 0; i < amount; ++i) {
        float x = this->random.nextFloat(-(float)amount / 2.0f, (float)amount / 2.0f);
        float y = this->random.nextFloat(-3.0f, 3.0f);
        float z = this->random.nextFloat(-(float)amount / 2.0f, (float)amount / 2.0f);
        TempFish* fish = new TempFish(context, position + glm::vec3(x, y, z));
        fish->world = w;
        this->boidList.push_back(fish);
    }
    this->target = glm::vec3(robot->position);
}

Boids::~Boids() {
    for (auto boid : this->boidList) {
        delete boid;
    }
}

void Boids::draw(glm::mat4 mat) {
    for (auto boid : this->boidList) {
        boid->draw(mat);
    }
}

void Boids::drawShadow(glm::mat4 mat) {
    for (auto boid : this->boidList) {
        boid->drawShadow(mat);
    }
}

void Boids::update() {
    this->updateTarget();
    for (auto boid : this->boidList) {
        boid->update();
        this->swimTowardsTarget(boid);
        this->swimTowardsCenter(boid);
        this->avoidOthers(boid);
        this->matchVelocity(boid);
        this->limitSpeed(boid);
    }
}

void Boids::swimTowardsCenter(TempFish* boid) {
    int numNeighbors = 0;
    glm::vec3 center(0.0f);
    glm::vec3 position = glm::vec3(boid->rigidBody->getModelMatrix()[3]);

    for (auto otherBoid : this->boidList) {
        glm::vec3 otherPosition = glm::vec3(otherBoid->rigidBody->getModelMatrix()[3]);
        if (glm::length(position - otherPosition) < this->visualRange) {
            center += otherPosition;
            numNeighbors += 1;
        }
    }

    if (numNeighbors) {
        center /= numNeighbors;
        boid->rigidBody->addForce((center - position) * this->centeringFactor);
    }
}

void Boids::swimTowardsTarget(TempFish* boid) {
    glm::vec3 targetDirection = this->target - glm::vec3(boid->rigidBody->getModelMatrix()[3]);
    boid->rigidBody->addForce(targetDirection * 0.00002f * glm::length(targetDirection));
}

void Boids::avoidOthers(TempFish* boid) {
    glm::vec3 move(0.0f);
    glm::vec3 position = glm::vec3(boid->rigidBody->getModelMatrix()[3]);
    for (auto otherBoid : this->boidList) {
        if (otherBoid != boid) {
            glm::vec3 direction = position - glm::vec3(otherBoid->rigidBody->getModelMatrix()[3]);
            if (glm::length(direction) < this->minAvoidDistance) {
                move += direction;
            }
        }
    }
    boid->rigidBody->addForce(move * this->avoidFactor);
}

void Boids::matchVelocity(TempFish* boid) {
    int numNeighbors = 0;
    glm::vec3 averageVelocity(0.0f);
    glm::vec3 position = glm::vec3(boid->rigidBody->getModelMatrix()[3]);

    for (auto otherBoid : this->boidList) {
        glm::vec3 otherPosition = glm::vec3(otherBoid->rigidBody->getModelMatrix()[3]);
        if (glm::length(position - otherPosition) < this->visualRange) {
            averageVelocity += otherBoid->rigidBody->getLinearVelocity();
            numNeighbors += 1;
        }
    }

    if (numNeighbors) {
        averageVelocity /= numNeighbors;
        boid->rigidBody->addForce((averageVelocity - boid->rigidBody->getLinearVelocity()) * this->matchingFactor);
    }
}

void Boids::limitSpeed(TempFish* boid) {
    if (glm::length(boid->rigidBody->getLinearVelocity()) > 15.0f) {
        boid->rigidBody->addForce(-boid->rigidBody->getLinearVelocity());
    }
}

void Boids::addRandomMovement(TempFish* boid) {
    boid->rigidBody->addForce((glm::vec3(this->random.nextFloat(), this->random.nextFloat(), this->random.nextFloat()) - 1.0f) * 2.0f * this->randomFactor);
}

void Boids::updateTarget() {
    this->target = glm::vec3(robot->position);
}