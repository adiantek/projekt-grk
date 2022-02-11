#pragma once

#include <Render_Utils.h>

#include <Physics/RigidBody.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <vector>
#include <world/Object3D.hpp>
#include <world/World.hpp>

namespace fish {

template <class T>
class Boids : world::Object3D {
   public:
    Boids(unsigned int amount, glm::vec3 position, world::World* w);
    ~Boids();
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    void update() override;

    float centeringFactor = 0.03f;
    float avoidFactor = 0.035f;
    float matchingFactor = 0.005f;
    float randomFactor = 0.7f;
    float minAvoidDistance = 3.5f;
    float visualRange = 75.0f;
    float lastTargetChange = -6.0f;
    float maxSpeed = 17.0f;

   private:
    void swimTowardsCenter(T* boid);
    void swimTowardsTarget(T* boid);
    void avoidOthers(T* boid);
    void matchVelocity(T* boid);
    void limitSpeed(T* boid);
    void addRandomMovement(T* boid);
    void updateTarget();

    std::vector<T*> boidList;
    glm::vec3 target;
    Random random;
    world::World* world;
};

}  // namespace fish

#include <Fish/Boids.inl>