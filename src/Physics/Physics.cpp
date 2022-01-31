#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>
#include <Physics/SimulationEventCallback.hpp>
#include <Water/Water.hpp>
#include <glm/glm.hpp>
#include <vector>

#define PX_RELEASE(x) \
    if (x) {          \
        x->release(); \
        x = NULL;     \
    }

using namespace physics;

const float stepTime = 1.0f / 60.f;

static PxFilterFlags simulationFilterShader(PxFilterObjectAttributes attributes0,
                                            PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                            PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {
    pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eNOTIFY_TOUCH_FOUND;

    return physx::PxFilterFlag::eDEFAULT;
}

Physics::Physics(float gravity, world::World* world, ErrorCallback::LogLevel logLevel)
    : errorCallback(logLevel) {
    physicsObject = this;
    this->gravity = gravity;
    this->world = world;
    this->foundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->allocator, (PxErrorCallback&)this->errorCallback);
    this->physx = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true);
    this->cooking = PxCreateCooking(PX_PHYSICS_VERSION, *this->foundation, PxCookingParams(PxTolerancesScale()));

#ifdef EMSCRIPTEN
    dispatcher = PxDefaultCpuDispatcherCreate(0);
#else
    dispatcher = PxDefaultCpuDispatcherCreate(4);
#endif

    PxSceneDesc sceneDesc(this->physx->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -gravity, 0.0f);
    sceneDesc.cpuDispatcher = dispatcher;
    sceneDesc.filterShader = simulationFilterShader;
    sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
    sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;
    sceneDesc.simulationEventCallback = (PxSimulationEventCallback*)new SimulationEventCallback();

    this->scene = this->physx->createScene(sceneDesc);
}

Physics::~Physics() {
    PX_RELEASE(this->scene);
    PX_RELEASE(this->dispatcher);
    PX_RELEASE(this->physx);
    PX_RELEASE(this->foundation);
}

void Physics::update(float deltaTime) {
    this->timeToProccess += deltaTime;
    while (this->timeToProccess > 0.0f) {
        physx::PxU32 nbActors = this->scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
        if (nbActors > 0) {
            std::vector<PxRigidActor*> actors(nbActors);
            this->scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, (PxActor**)&actors[0], nbActors);
            for (auto actor : actors) {
                if (!actor->userData)
                    continue;
                physics::RigidBody* rigidBody = (physics::RigidBody*)actor->userData;
                if (!this->world->chunksLoaded(glm::vec3(rigidBody->getModelMatrix()[3]))) {
                    rigidBody->putToSleep();
                    continue;
                } else {
                    rigidBody->wakeUp();
                }
                if (((physx::PxMat44)actor->getGlobalPose()).column3.y < waterObject->getY()) {
                    rigidBody->addForce(glm::vec3(0.0f, rigidBody->getMass() / rigidBody->density * this->gravity * 0.997f, 0.0f));
                    rigidBody->applyDrag(0.997f);
                } else {
                    rigidBody->applyDrag(0.001225f);
                }
            }
        }

        this->scene->simulate(stepTime);
        this->scene->fetchResults(true);
        this->timeToProccess -= stepTime;
    }
}

PxRigidBody* Physics::createRigidBody(bool isStatic, PxTransform& pose, PxGeometry& geometry, void* object, float staticFriction, float dynamicFriction, float restitution) {
    PxMaterial* material = this->physx->createMaterial(staticFriction, dynamicFriction, restitution);
    PxRigidBody* rigidBody = isStatic ? (PxRigidBody*)this->physx->createRigidStatic(pose) : (PxRigidBody*)this->physx->createRigidDynamic(pose);
    PxShape* shape = this->physx->createShape(geometry, *material);
    rigidBody->attachShape(*shape);
    rigidBody->userData = object;
    PX_RELEASE(shape);
    PX_RELEASE(material);
    this->scene->addActor(*rigidBody);
    return rigidBody;
}

void Physics::deleteRigidBody(PxRigidBody* rigidBody) {
    this->scene->removeActor(*rigidBody);
    PX_RELEASE(rigidBody);
}

physx::PxTriangleMeshGeometry Physics::createTriangleGeometry(float* vertices, int verticesNumber, int* indices, int trianglesNumber) {
    physx::PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = verticesNumber;
    meshDesc.points.stride = sizeof(physx::PxVec3);
    meshDesc.points.data = vertices;

    meshDesc.triangles.count = trianglesNumber;
    meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
    meshDesc.triangles.data = indices;

    physx::PxTriangleMeshGeometry triGeom;
    triGeom.triangleMesh = this->cooking->createTriangleMesh(meshDesc, this->physx->getPhysicsInsertionCallback());

    return triGeom;
}

physx::PxTriangleMeshGeometry Physics::createTriangleGeometry(vertex::VertexBuffer *vb, int* indices, int trianglesNumber) {
    physx::PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = vb->getVertices();
    meshDesc.points.stride = vb->getFormat()->getGPUSize();
    meshDesc.points.data = vb->buff + vb->getFormat()->pos;

    meshDesc.triangles.count = trianglesNumber;
    meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
    meshDesc.triangles.data = indices;

    physx::PxTriangleMeshGeometry triGeom;
    triGeom.triangleMesh = this->cooking->createTriangleMesh(meshDesc, this->physx->getPhysicsInsertionCallback());

    return triGeom;
}

physics::Physics* physicsObject;
