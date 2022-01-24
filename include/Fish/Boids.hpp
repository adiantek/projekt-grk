#pragma once

#include <world/Object3D.hpp>
#include <glm/glm.hpp>
#include <Physics/RigidBody.hpp>
#include <world/World.hpp>
#include <Render_Utils.h>
#include <vector>
#include <algorithm>

template<class T>
class Boids : world::Object3D {
   public:
    Boids(unsigned int amount, glm::vec3 position, world::World* w);
    ~Boids();
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    void update() override;

   private:
    void swimTowardsCenter(T* boid);
    void swimTowardsTarget(T* boid);
    void avoidOthers(T* boid);
    void matchVelocity(T* boid);
    void limitSpeed(T* boid);
    void addRandomMovement(T* boid);
    void updateTarget();

    float centeringFactor = 0.015f;
    float avoidFactor = 0.07f;
    float matchingFactor = 0.005f;
    float randomFactor = 0.7f;
    float minAvoidDistance = 7.0f;
    float visualRange = 75.0f;
    std::vector<T*> boidList;
    glm::vec3 target;
    Random random;
};

#include <Fish/Boids.cpp>