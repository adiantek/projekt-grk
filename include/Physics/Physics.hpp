#pragma once

#include <PxPhysicsAPI.h>

#include <Physics/ErrorCallback.hpp>
#include <world/Object3D.hpp>
namespace physics {
class Physics {
   public:
    Physics(float gravity, ErrorCallback::LogLevel logLevel = ErrorCallback::__DEBUG);
    ~Physics();
    void update(float deltaTime);
    physx::PxRigidBody* createRigidBody(bool isStatic, physx::PxTransform& pose, physx::PxGeometry& geometry, world::Object3D* object, float staticFriction, float dynamicFriction, float restitution);
    void deleteRigidBody(physx::PxRigidBody* rigidBody);

   private:
    physx::PxDefaultAllocator allocator;
    ErrorCallback errorCallback;
    physx::PxFoundation* foundation = nullptr;
    physx::PxPhysics* physx = nullptr;
    physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
    physx::PxScene* scene = nullptr;
    float timeToProccess = 0.0f;
};
}  // namespace physics

extern physics::Physics* physicsObject;