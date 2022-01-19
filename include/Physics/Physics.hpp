#pragma once

#include <PxPhysicsAPI.h>

#include <Physics/ErrorCallback.hpp>

using namespace physx;

class Physics {
   public:
    Physics(float gravity, ErrorCallback::LogLevel logLevel = ErrorCallback::__DEBUG);
    ~Physics();
    void update(float deltaTime);

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