#include <Physics/Physics.hpp>
#include <Physics/SimulationEventCallback.hpp>

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL; }

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
    this->scene->simulate(deltaTime);
    this->scene->fetchResults(true);
}

#undef PX_RELEASE