#include <Physics/SimulationEventCallback.hpp>
#include <Physics/RigidBody.hpp>
namespace physics {
void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    ((RigidBody*)pairHeader.actors[0]->userData)->object->onContact(pairHeader.actors[1], pairs, nbPairs);
    ((RigidBody*)pairHeader.actors[1]->userData)->object->onContact(pairHeader.actors[0], pairs, nbPairs);
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {
}

void SimulationEventCallback::onWake(PxActor** actors, PxU32 count) {
}

void SimulationEventCallback::onSleep(PxActor** actors, PxU32 count) {
}

void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
}

void SimulationEventCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {
}
}  // namespace physics