#include <world/Object3D.hpp>
#include <vector>
#include <Logger.h>

using namespace world;

Object3D::Object3D() {
}

Object3D::~Object3D() {
}

void Object3D::update() {
}

void Object3D::draw(glm::mat4 mat) {
}

void Object3D::drawShadow(glm::mat4 mat) {
}

void Object3D::onContact(const physx::PxRigidActor* actor, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
    // LOGI("Contact with: %p", actor);
    // for (physx::PxU32 i = 0; i < nbPairs; i++) {
    //     const physx::PxContactPair& cp = pairs[i];
    //     std::vector<physx::PxContactPairPoint> buffer(cp.contactCount);
    //     LOGI("Contact pair points number: %d", cp.contactCount);
    //     LOGI("Contact point positions: ");
    //     cp.extractContacts(buffer.data(), cp.contactCount * sizeof(physx::PxContactPairPoint));
    //     for (auto & i : buffer)
    //         LOGI("Position: %f %f %f", i.position.x, i.position.y, i.position.z);
    // }
}