#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>
#include <Physics/SimulationEventCallback.hpp>
#include <Water/Water.hpp>
#include <Camera/Camera.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <utils/Gizmos.hpp>
#include <Glow/GlowShader.hpp>
#include <Controller/Controller.hpp>
#include <ResourceLoader.hpp>
#include <vector>
#include <Logger.h>

#define PX_RELEASE(x) \
    if (x) {          \
        x->release(); \
        x = NULL;     \
    }

#define GRAB_DIST 100.0f
#define GLOW_DIST 100.0f
#define PLACE_DIST 20.0f

using namespace physics;

enum ActiveGroup {
    NONRAYHITABBLE = (1<<0),
    RAYHITABBLE = (1<<1)
};

const float stepTime = 1.0f / 60.f;

static PxFilterFlags simulationFilterShader(PxFilterObjectAttributes attributes0,
                                            PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                            PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {
    pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eNOTIFY_TOUCH_FOUND;

    return physx::PxFilterFlag::eDEFAULT;
}

Physics::Physics(float gravity, ErrorCallback::LogLevel logLevel)
    : errorCallback(logLevel) {
    physicsObject = this;
    this->gravity = gravity;
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

    this->models.push_back(resourceLoaderExternal->m_primitives_cube);
    this->models.push_back(resourceLoaderExternal->m_primitives_cylinder);
    this->models.push_back(resourceLoaderExternal->m_primitives_sphere);
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
                if (rigidBody->grabbed) {
                    glm::vec3 direction = robot->position - glm::vec3(rigidBody->getModelMatrix()[3]);
                    ((physx::PxRigidBody*)actor)->setLinearVelocity(physx::PxVec3(direction.x, direction.y, direction.z));
                }
            }
        }

        this->scene->simulate(stepTime);
        this->scene->fetchResults(true);
        this->timeToProccess -= stepTime;
    }
}

PxRigidBody* Physics::createRigidBody(bool isStatic, PxTransform& pose, PxGeometry& geometry, void* object, float staticFriction, float dynamicFriction, float restitution, bool grabbable) {
    PxMaterial* material = this->physx->createMaterial(staticFriction, dynamicFriction, restitution);
    PxRigidBody* rigidBody = isStatic ? (PxRigidBody*)this->physx->createRigidStatic(pose) : (PxRigidBody*)this->physx->createRigidDynamic(pose);
    PxShape* shape = this->physx->createShape(geometry, *material);
    PxFilterData filterData;
    filterData.word0 = grabbable ? RAYHITABBLE : NONRAYHITABBLE;
    shape->setQueryFilterData(filterData);
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

physx::PxTriangleMeshGeometry Physics::createTriangleGeometry(float* vertices, unsigned int verticesNumber, int* indices, unsigned int trianglesNumber) {
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

physx::PxTriangleMeshGeometry Physics::createTriangleGeometry(vertex::VertexBuffer *vb, int* indices, unsigned int trianglesNumber) {
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

std::pair<physx::PxVec3, physx::PxVec3> calculateRay() {
    glm::vec4 start(0.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 end(0.0f, 0.0f, 1.0f, 1.0f);

    glm::mat4 viewInverse = glm::inverse(camera->getTransformationMatrix());

    start = viewInverse * start;
    end = viewInverse * end;

    start /= start.w;
    end /= end.w;

    glm::vec4 dir = glm::normalize(end - start);

    return std::make_pair(physx::PxVec3(start.x, start.y, start.z), physx::PxVec3(dir.x, dir.y, dir.z));
}

void Physics::grab() {
    std::pair<physx::PxVec3, physx::PxVec3> pair = calculateRay();

    physx::PxVec3 position = pair.first;
    physx::PxVec3 direction = pair.second;

    PxRaycastBuffer hit;
    PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC);
    filterData.data.word0 = RAYHITABBLE;
    this->scene->raycast(position, direction, GRAB_DIST, hit, ((physx::PxHitFlags)(PxHitFlag::eDEFAULT)), filterData);

    if (hit.hasAnyHits()) {
        ((RigidBody*)hit.block.actor->userData)->grabbed = !((RigidBody*)hit.block.actor->userData)->grabbed;
    }
}

void Physics::grabMultiple() {
    std::pair<physx::PxVec3, physx::PxVec3> pair = calculateRay();

    physx::PxTransform position(pair.first);
    physx::PxVec3 direction = pair.second;

    PxSweepHit hitBuffer[30];
    PxSweepBuffer hit(hitBuffer, 30);
    PxSphereGeometry geometry(5.0f);
    PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eNO_BLOCK);
    filterData.data.word0 = RAYHITABBLE;
    this->scene->sweep(geometry, position, direction, GRAB_DIST, hit, ((physx::PxHitFlags)(PxHitFlag::eDEFAULT)), filterData);

    if (hit.hasAnyHits()) {
        unsigned int hitAmount = hit.getNbAnyHits();
        for (unsigned int i = 0; i < hitAmount; ++i) {
            RigidBody* rigidBody = (RigidBody*)hit.getAnyHit(i).actor->userData;
            rigidBody->grabbed = !rigidBody->grabbed;
        }
    }
}

void Physics::place() {
    std::pair<physx::PxVec3, physx::PxVec3> pair = calculateRay();

    physx::PxVec3 position = pair.first;
    physx::PxVec3 direction = pair.second;

    PxRaycastBuffer hit;
    PxQueryFilterData filterData;
    filterData.data.word0 = RAYHITABBLE;
    this->scene->raycast(position, direction, PLACE_DIST, hit, ((physx::PxHitFlags)(PxHitFlag::eDEFAULT)), filterData);

    glm::mat4 model = glm::translate(glm::vec3(position.x, position.y, position.z) + PLACE_DIST * glm::vec3(direction.x, direction.y, direction.z));
    if (hit.hasAnyHits()) {
        glm::vec3 normal = glm::vec3(hit.block.normal.x, hit.block.normal.y, hit.block.normal.z);
        glm::vec3 position = glm::vec3(hit.block.position.x, hit.block.position.y + 1.0f, hit.block.position.z) + 0.1f * normal;
        model = glm::translate(position) * glm::transpose(glm::lookAt(glm::vec3(0.0f), normal, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    this->blocks.push_back(new Cubefish(model, 3.0f, 3.0f, this->models[currentModel]));
}

void Physics::draw(glm::mat4 mat) {
    std::pair<physx::PxVec3, physx::PxVec3> pair = calculateRay();

    for (auto cube : this->blocks) {
        cube->draw(mat);
    }

    if (controller->mouseRightClicked) {
        if (controller->scopeMode == 1) {
            physx::PxTransform position(pair.first);
            physx::PxVec3 direction = pair.second;

            PxSweepHit hitBuffer[30];
            PxSweepBuffer hit(hitBuffer, 30);
            PxSphereGeometry geometry(5.0f);
            PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eNO_BLOCK);
            filterData.data.word0 = RAYHITABBLE;
            this->scene->sweep(geometry, position, direction, GRAB_DIST, hit, ((physx::PxHitFlags)(PxHitFlag::eDEFAULT)), filterData);

            if (hit.hasAnyHits()) {
                unsigned int hitAmount = hit.getNbAnyHits();
                Glow::glow->startFB();
                for (unsigned int i = 0; i < hitAmount; ++i) {
                    ((RigidBody*)hit.getAnyHit(i).actor->userData)->object->drawShadow(mat);
                }
                Glow::glow->stopFB();
            }
        } else if (controller->scopeMode == 0) {
            physx::PxVec3 position = pair.first;
            physx::PxVec3 direction = pair.second;

            PxRaycastBuffer hit;
            PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC);
            filterData.data.word0 = RAYHITABBLE;
            this->scene->raycast(position, direction, GRAB_DIST, hit, ((physx::PxHitFlags)(PxHitFlag::eDEFAULT)), filterData);

            if (hit.hasAnyHits()) {
                Glow::glow->startFB();
                ((RigidBody*)hit.block.actor->userData)->object->drawShadow(mat);
                Glow::glow->stopFB();
            }
        } else {
            physx::PxVec3 position = pair.first;
            physx::PxVec3 direction = pair.second;

            PxRaycastBuffer hit;
            PxQueryFilterData filterData;
            filterData.data.word0 = RAYHITABBLE;
            this->scene->raycast(position, direction, PLACE_DIST, hit, ((physx::PxHitFlags)(PxHitFlag::eDEFAULT)), filterData);

            glm::mat4 model = glm::translate(glm::vec3(position.x, position.y, position.z) + PLACE_DIST * glm::vec3(direction.x, direction.y, direction.z));

            if (hit.hasAnyHits()) {
                model = glm::translate(glm::vec3(hit.block.position.x, hit.block.position.y + 1.0f, hit.block.position.z)) * glm::transpose(glm::lookAt(glm::vec3(0.0f), glm::vec3(hit.block.normal.x, hit.block.normal.y, hit.block.normal.z), glm::vec3(0.0f, 1.0f, 0.0f)));
            }
            glm::mat4 transformation = mat * model;

            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendColor(0.1f, 1.0f, 0.1f, 1.0f);
	        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
            glUseProgram(resourceLoaderExternal->p_cubefish);
            glUniform1i(resourceLoaderExternal->p_cubefish_uni_colorTexture, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_dummy);
            glUniform1i(resourceLoaderExternal->p_cubefish_uni_normalSampler, 1);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_normal);
            glUniform1i(resourceLoaderExternal->p_cubefish_uni_caustics, 2);
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
            glUniform1i(resourceLoaderExternal->p_cubefish_uni_depthMap, 3);
            glActiveTexture(GL_TEXTURE0 + 3);
            glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_height);
            glUniform1i(resourceLoaderExternal->p_cubefish_uni_roughnessMap, 4);
            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_roughness);
            glUniform1i(resourceLoaderExternal->p_cubefish_uni_aoMap, 5);
            glActiveTexture(GL_TEXTURE0 + 5);
            glBindTexture(GL_TEXTURE_2D, resourceLoaderExternal->tex_wall_ao);
            glUniformMatrix4fv(resourceLoaderExternal->p_cubefish_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(resourceLoaderExternal->p_cubefish_uni_transformation, 1, GL_FALSE, glm::value_ptr(transformation));
            glUniformMatrix4fv(resourceLoaderExternal->p_cubefish_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));
            for (auto mesh : this->models[currentModel]->getMeshes()) {
                Core::DrawContext(*mesh->getRenderContext());
            }
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);
        }
    }

    physx::PxU32 nbActors = this->scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
    if (nbActors > 0) {
        std::vector<PxRigidActor*> actors(nbActors);
        this->scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, (PxActor**)&actors[0], nbActors);
        for (auto actor : actors) {
            if (!actor->userData)
                continue;
            physics::RigidBody* rigidBody = (physics::RigidBody*)actor->userData;
            if (rigidBody->grabbed) {
                glm::vec3 direction = robot->position - glm::vec3(rigidBody->getModelMatrix()[3]);
                utils::Gizmos::line(robot->position + glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(rigidBody->getModelMatrix()[3]), glm::vec3(0.0f));
            }
        }
    }
}

void Physics::nextModel() {
    this->currentModel += 1;
    this->currentModel %= this->models.size();
}

physics::Physics* physicsObject;
