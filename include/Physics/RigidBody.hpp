#pragma once

#include <PxPhysicsAPI.h>

#include <glm/glm.hpp>
#include <world/Object3D.hpp>

namespace physics {
class RigidBody {
   public:
    RigidBody(bool isStatic, physx::PxTransform& pose, physx::PxGeometry& geometry, world::Object3D* object, float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.6f);
    ~RigidBody();
    glm::mat4 getModelMatrix();
    void setMass(float mass);
    void addForce(glm::vec3 force);
    void addTorque(glm::vec3 torque);

   private:
    physx::PxRigidBody* inner;
};
}  // namespace physics