#pragma once

#include <PxPhysicsAPI.h>

#include <Physics/ErrorCallback.hpp>
#include <world/Object3D.hpp>

using namespace physx;

class Physics {
   public:
    Physics(float gravity, ErrorCallback::LogLevel logLevel = ErrorCallback::__DEBUG);
    ~Physics();
    void update(float deltaTime);
    PxRigidBody* createRigidBody(bool isStatic, PxTransform &pose, PxGeometry &geometry, world::Object3D* object, float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.6f);
    void deleteRigidBody(PxRigidBody* rigidBody);

   private:
    PxDefaultAllocator allocator;
    ErrorCallback errorCallback;
    PxFoundation* foundation = nullptr;
    PxPhysics* physx = nullptr;
    PxDefaultCpuDispatcher*	dispatcher = nullptr;
    PxScene* scene = nullptr;
    float timeToProccess = 0.0f;
};

extern Physics* physics = nullptr;