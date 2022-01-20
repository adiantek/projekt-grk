#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>

namespace physics {
RigidBody::RigidBody(bool isStatic, physx::PxTransform &pose, physx::PxGeometry &geometry, world::Object3D *object, float staticFriction, float dynamicFriction, float restitution) {
    this->object = object;
    this->inner = physicsObject->createRigidBody(isStatic, pose, geometry, this, staticFriction, dynamicFriction, restitution);
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

void RigidBody::setMass(float mass) {
    this->inner->setMass(mass);
}

void RigidBody::addForce(glm::vec3 force) {
    this->inner->addForce(physx::PxVec3(force.x, force.y, force.z));
}

void RigidBody::addTorque(glm::vec3 torque) {
    this->inner->addTorque(physx::PxVec3(torque.x, torque.y, torque.z));
}
}  // namespace physics
