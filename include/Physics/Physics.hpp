#pragma once

#include <PxConfig.h>
#include <PxPhysicsAPI.h>

#include <Physics/ErrorCallback.hpp>
#include <vector>
#include <vertex/VertexBuffer.hpp>
#include <world/Object3D.hpp>
#include <world/World.hpp>

namespace physics {
class Physics {
   public:
    Physics(float gravity, ErrorCallback::LogLevel logLevel = ErrorCallback::__DEBUG);
    ~Physics();
    void update(float deltaTime);
    physx::PxRigidBody* createRigidBody(bool isStatic, physx::PxTransform& pose, physx::PxGeometry& geometry, void* object, float staticFriction, float dynamicFriction, float restitution, bool grabbable);
    void deleteRigidBody(physx::PxRigidBody* rigidBody);
    physx::PxTriangleMeshGeometry createTriangleGeometry(float* vertices, unsigned int verticesNumber, int* indices, unsigned int trianglesNumber);
    physx::PxTriangleMeshGeometry createTriangleGeometry(vertex::VertexBuffer* vb, int* indices, unsigned int trianglesNumber);
    physx::PxHeightField* createHeightField(physx::PxHeightFieldDesc &description);
    void grab();
    void grabMultiple();
    void draw(glm::mat4 mat);
    world::World* world = nullptr;
    physx::PxScene* scene = nullptr;

   private:
    physx::PxDefaultAllocator allocator;
    ErrorCallback errorCallback;
    float gravity;
    physx::PxFoundation* foundation = nullptr;
    physx::PxPhysics* physx = nullptr;
    physx::PxCooking* cooking = nullptr;
    physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
    float timeToProccess = 0.0f;
};
}  // namespace physics

extern physics::Physics* physicsObject;