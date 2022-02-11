#include <Fish/Boids.hpp>
#include <Time/Time.hpp>
#include <ResourceLoader.hpp>
#include <Random.hpp>
#include <Robot/Robot.hpp>
#include <Water/Water.hpp>
#include <world/ChunkPosition.hpp>
#include <world/World.hpp>

long long static seeder = 0L;

template<class T>
fish::Boids<T>::Boids(unsigned int amount, glm::vec3 position, world::World* w) : random(seeder) {;
    seeder++;
    for (unsigned int i = 0; i < amount; ++i) {
        float x = this->random.nextFloat(-(float)amount / 3.0f, (float)amount / 3.0f);
        float y = this->random.nextFloat(-4.0f, 4.0f);
        float z = this->random.nextFloat(-(float)amount / 3.0f, (float)amount / 3.0f);
        this->boidList.push_back(new T(position + glm::vec3(x, y, z), w));
    }
    this->world = w;
    this->target = glm::vec3(robot->position);
}

template<class T>
fish::Boids<T>::~Boids() {
    for (auto boid : this->boidList) {
        delete boid;
    }
}

template<class T>
void fish::Boids<T>::draw(glm::mat4 mat) {
    for (auto boid : this->boidList) {
        boid->draw(mat);
    }
}

template<class T>
void fish::Boids<T>::drawShadow(glm::mat4 mat) {
    for (auto boid : this->boidList) {
        boid->drawShadow(mat);
    }
}

template<class T>
void fish::Boids<T>::update() {
    this->updateTarget();
    for (auto boid : this->boidList) {
        this->swimTowardsCenter(boid);
        boid->update();
        this->swimTowardsTarget(boid);
        this->avoidOthers(boid);
        this->matchVelocity(boid);
        this->limitSpeed(boid);
    }
}

template<class T>
void fish::Boids<T>::swimTowardsCenter(T* boid) {
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

template<class T>
void fish::Boids<T>::swimTowardsTarget(T* boid) {
    boid->setTarget(this->target);
}

template<class T>
void fish::Boids<T>::avoidOthers(T* boid) {
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

template<class T>
void fish::Boids<T>::matchVelocity(T* boid) {
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

template<class T>
void fish::Boids<T>::limitSpeed(T* boid) {
    if (glm::length(boid->rigidBody->getLinearVelocity()) > this->maxSpeed) {
        boid->rigidBody->addForce(-boid->rigidBody->getLinearVelocity());
    }
}

template<class T>
void fish::Boids<T>::addRandomMovement(T* boid) {
    boid->rigidBody->addForce((glm::vec3(this->random.nextFloat(), this->random.nextFloat(), this->random.nextFloat()) - 1.0f) * 2.0f * this->randomFactor);
}

template<class T>
void fish::Boids<T>::updateTarget() {
    if ((float)timeExternal->lastFrame - this->lastTargetChange > 2.0f) {
        this->target = this->boidList[0]->findTarget();
        this->lastTargetChange = (float)timeExternal->lastFrame;
    }
}