#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Particle/Particle.hpp>
#include <Robot/Robot.hpp>
#include <Time/Time.hpp>
#include <glm/ext.hpp>


Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeRemaining, float rotation, float scale) {
    this->position = position;
    this->velocity = velocity;
    this->gravityEffect = gravityEffect;
    this->lifeRemaining = lifeRemaining;
    this->rotation = rotation;
    this->scale = scale;
}

Particle::Particle() {
    this->position = glm::vec3(robot->position.x, robot->position.y, robot->position.z);
    this->velocity = glm::vec3(0.1f);
}

Particle::~Particle() {
}

glm::vec3 Particle::getPosition() {
    return this->position;
}

float Particle::getScale() {
    return this->scale;
}

float Particle::getRotation() {
    return this->rotation;
}

bool Particle::isAlive() {
    return lifeRemaining < lifeLength;
}