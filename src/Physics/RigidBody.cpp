#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>

namespace physics {
RigidBody::RigidBody(bool isStatic, physx::PxTransform &pose, physx::PxGeometry &geometry, world::Object3D *object, float staticFriction, float dynamicFriction, float restitution, bool kinematic, bool grabbable) {
    this->object = object;
    this->inner = physicsObject->createRigidBody(isStatic, pose, geometry, this, staticFriction, dynamicFriction, restitution, grabbable && !kinematic);
    this->kinematic = kinematic;
    if (kinematic) {
        this->inner->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
    }
}

RigidBody::~RigidBody() {
    physicsObject->deleteRigidBody(this->inner);
}

glm::mat4 RigidBody::getModelMatrix() {
    physx::PxMat44 transform = this->inner->getGlobalPose();
    return glm::mat4(
        transform.column0.x, transform.column0.y, transform.column0.z, transform.column0.w,
        transform.column1.x, transform.column1.y, transform.column1.z, transform.column1.w,
        transform.column2.x, transform.column2.y, transform.column2.z, transform.column2.w,
        transform.column3.x, transform.column3.y, transform.column3.z, transform.column3.w);
}

float RigidBody::getMass() {
    return this->inner->getMass();
}

glm::vec3 RigidBody::getLinearVelocity() {
    physx::PxVec3 velocity = this->inner->getLinearVelocity();
    return glm::vec3(velocity.x, velocity.y, velocity.z);
}

void RigidBody::setMass(float mass) {
    this->inner->setMass(mass);
}

void RigidBody::setLinearVelocity(glm::vec3 velocity) {
    if (!this->grabbed && !this->kinematic) {
        this->inner->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
    }
}

void RigidBody::setKinematicTarget(glm::mat4 destinationPose) {
    if (this->kinematic) {
        physx::PxMat44 dest(glm::value_ptr(destinationPose));
        ((physx::PxRigidDynamic*)this->inner)->setKinematicTarget(physx::PxTransform(dest));
    }
}

void RigidBody::addForce(glm::vec3 force, physx::PxForceMode::Enum mode) {
    if (!this->grabbed && !this->kinematic) {
        this->inner->addForce(physx::PxVec3(force.x, force.y, force.z), mode);
    }
}

void RigidBody::addTorque(glm::vec3 torque) {
    if (!this->grabbed && !this->kinematic) {
        this->inner->addTorque(physx::PxVec3(torque.x, torque.y, torque.z));
    }
}

void RigidBody::applyDrag(float density) {
    if (this->drag && !this->kinematic) {
        physx::PxVec3 velocityDirection = this->inner->getLinearVelocity();
        physx::PxVec3 velocityAngularDirection = this->inner->getAngularVelocity();
        float speed = velocityDirection.normalize();
        float angularSpeed = velocityAngularDirection.normalize();
        float force = -0.5f * speed * speed * density;
        float torque = -0.5f * angularSpeed * angularSpeed * density;
        this->inner->addForce(velocityDirection * force);
        this->inner->addTorque(velocityAngularDirection * torque);
    }
}

void RigidBody::putToSleep() {
    if (!this->sleep) {
        this->prevState = State {
            this->inner->getLinearVelocity(),
            this->inner->getAngularVelocity()
        };
        this->inner->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
        this->sleep = true;
    }
}

void RigidBody::wakeUp() {
    if (this->sleep) {
        this->inner->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
        this->inner->setLinearVelocity(this->prevState.velocity);
        this->inner->setAngularVelocity(this->prevState.angularVelocity);
        this->sleep = false;
    }
}

void RigidBody::rotateForward(glm::mat4 rot) {
    if (!this->grabbed && !this->kinematic) {
        glm::vec3 velocity = this->getLinearVelocity();
        if (glm::length(velocity) > 0.0f) {
            auto quat = glm::quat_cast(glm::transpose(glm::lookAt(glm::vec3(0.0f), velocity, glm::vec3(0.0f, 1.0f, 0.0f))) * rot);
            this->inner->setGlobalPose(physx::PxTransform(this->inner->getGlobalPose().p, physx::PxQuat(quat.x, quat.y, quat.z, quat.w)));
        }
    }
}
}  // namespace physics
