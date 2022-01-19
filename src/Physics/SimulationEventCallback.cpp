#include <Physics/SimulationEventCallback.hpp>

void SimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    // HINT: You can check which actors are in contact
    // using pairHeader.actors[0] and pairHeader.actors[1]
    printf("Contact pair: %p %p\n", pairHeader.actors[0], pairHeader.actors[1]);
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];
        //std::vector<PxContactPairPoint> buffer(cp.contactCount);
        printf("Contact pair points: %d\n", cp.contactCount);
        
        // HINT: two get the contact points, use
        // PxContactPair::extractContacts
        //cp.extractContacts(buffer.data(), cp.contactCount * sizeof(PxContactPairPoint));
        //printf("Contact point positions: ");
        //for (auto & i : buffer)
        //    printf("%f %f %f, ", i.position.x, i.position.y, i.position.z);
        //printf("\n");
        // You need to provide the function with a buffer
        // in which the contact points will be stored.
        // Create an array (vector) of type PxContactPairPoint
        // The number of elements in array should be at least
        // cp.contactCount (which is the number of contact points)
        // You also need to provide the function with the
        // size of the buffer (it should equal the size of the
        // array in bytes)
        // Finally, for every extracted point, you can access
        // its details, such as position
    }
    /*if (pairHeader.actors[1] == sphereBody) {
        printf("KULA KOSTKA\n");
        int index = -1;
        for (int i = 0; i < boxBodies.size(); ++i) {
            if (boxBodies[i] == pairHeader.actors[0]) {
                index = i;
                boxBodiesDraw[i + 1] = false;
                garbage.push_back(boxBodies[i]);
                //pxScene.scene->removeActor(*);
                break;
            }
        }
    }
    if (pairHeader.actors[0] == sphereBody) {
        printf("KULA KOSTKA\n");
        int index = -1;
        for (int i = 0; i < boxBodies.size(); ++i) {
            if (boxBodies[i] == pairHeader.actors[1]) {
                index = i;
                boxBodiesDraw[i + 1] = false;
                garbage.push_back(boxBodies[i]);
                //pxScene.scene->removeActor(*boxBodies[i]);
                break;
            }
        }
    }*/
}

void SimulationEventCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {

}

void SimulationEventCallback::onWake(PxActor** actors, PxU32 count) {

}

void SimulationEventCallback::onSleep(PxActor** actors, PxU32 count) {

}

void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {

}

void SimulationEventCallback::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {

}
