#pragma once

#include <PxPhysicsAPI.h>

#include <glm/glm.hpp>
#include <world/Object3D.hpp>

struct State {
    physx::PxVec3 velocity;
    physx::PxVec3 angularVelocity;
};

namespace physics {
class RigidBody {
   public:
    RigidBody(bool isStatic, physx::PxTransform& pose, physx::PxGeometry& geometry, world::Object3D* object, float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.6f);
    ~RigidBody();
    glm::mat4 getModelMatrix();
    float getMass();
    void setMass(float mass);
    void addForce(glm::vec3 force);
    void addTorque(glm::vec3 torque);
    void applyDrag(float density);
    void putToSleep();
    void wakeUp();
    world::Object3D* object;
    float density = 1.0f;

   private:
    physx::PxRigidBody* inner;
    bool sleep = false;
    State prevState;
};
}  // namespace physics