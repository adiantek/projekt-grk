#include <Logger.h>

#include <Physics/Physics.hpp>
#include <Physics/SimulationEventCallback.hpp>
#include <glm/glm.hpp>
#include <vector>

#define PX_RELEASE(x) if (x) { x->release(); x = NULL; }

const float stepTime = 1.0f / 60.f;

static PxFilterFlags simulationFilterShader(PxFilterObjectAttributes attributes0,
                                            PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                            PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {
    pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eNOTIFY_TOUCH_FOUND;

    return physx::PxFilterFlag::eDEFAULT;
}

Physics::Physics(float gravity, ErrorCallback::LogLevel logLevel)
    : errorCallback(logLevel) {
    physics = this;
    this->foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, (PxErrorCallback&)this->errorCallback);
    this->physx = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true);

    dispatcher = PxDefaultCpuDispatcherCreate(4);

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
        this->scene->simulate(stepTime);
        this->scene->fetchResults(true);
        this->timeToProccess -= stepTime;
    }
    auto actorFlags = PxActorTypeFlag::eRIGID_DYNAMIC;// | PxActorTypeFlag::eRIGID_STATIC;
    PxU32 actorCount = this->scene->getNbActors(actorFlags);
    if (actorCount == 0) {
        return;
    }
    std::vector<PxRigidActor*> actorList(actorCount);
    this->scene->getActors(actorFlags, (PxActor**)&actorList[0], actorCount);
    for (auto actor : actorList) {
        if (!actor->userData) {
            LOGW("PhysX actor without user data");
            continue;
        }
        world::Object3D* object = (world::Object3D*)actor->userData;
        PxMat44 transform = actor->getGlobalPose();

        object->updateModelMatrix(glm::mat4(
            transform.column0.x, transform.column0.y, transform.column0.z, transform.column0.w,
            transform.column1.x, transform.column1.y, transform.column1.z, transform.column1.w,
            transform.column2.x, transform.column2.y, transform.column2.z, transform.column2.w,
            transform.column3.x, transform.column3.y, transform.column3.z, transform.column3.w));
    }
}

PxRigidBody* Physics::createRigidBody(bool isStatic, PxTransform &pose, PxGeometry &geometry, world::Object3D* object, float staticFriction, float dynamicFriction, float restitution) {
    PxMaterial* material = this->physx->createMaterial(staticFriction, dynamicFriction, restitution);
    PxRigidBody* rigidBody = isStatic ? (PxRigidBody*)this->physx->createRigidStatic(pose) : (PxRigidBody*)this->physx->createRigidDynamic(pose);
    PxShape* shape = this->physx->createShape(geometry, *material);
    rigidBody->attachShape(*shape);
    PX_RELEASE(shape);
    PX_RELEASE(material);
    this->scene->addActor(*rigidBody);
    
    return nullptr;
}

void Physics::deleteRigidBody(PxRigidBody* rigidBody) {
    this->scene->removeActor(*rigidBody);
    PX_RELEASE(rigidBody);
}

#undef PX_RELEASE