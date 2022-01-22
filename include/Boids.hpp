#pragma once

#include <world/Object3D.hpp>
#include <glm/glm.hpp>
#include <Physics/RigidBody.hpp>
#include <world/World.hpp>
#include <Render_Utils.h>
#include <vector>
#include <algorithm>

class TempFish : world::Object3D {
   public:
    TempFish(Core::RenderContext context, glm::vec3 initPos);
    ~TempFish();
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    void update() override;
    Core::RenderContext context;
    physics::RigidBody* rigidBody;
    world::World* world;
};

class Boids : world::Object3D {
   public:
    Boids(unsigned int amount, Core::RenderContext context, glm::vec3 position, world::World* w);
    ~Boids();
    void draw(glm::mat4 mat) override;
    void drawShadow(glm::mat4 mat) override;
    void update() override;

   private:
    void swimTowardsCenter(TempFish* boid);
    void swimTowardsTarget(TempFish* boid);
    void avoidOthers(TempFish* boid);
    void matchVelocity(TempFish* boid);
    void limitSpeed(TempFish* boid);
    void addRandomMovement(TempFish* boid);
    void updateTarget();

    float centeringFactor = 0.015f;
    float avoidFactor = 0.07f;
    float matchingFactor = 0.005f;
    float randomFactor = 0.7f;
    float minAvoidDistance = 7.0f;
    float visualRange = 75.0f;
    std::vector<TempFish*> boidList;
    glm::vec3 target;
    Random random;
};