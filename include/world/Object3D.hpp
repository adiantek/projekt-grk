#pragma once

#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

namespace world {

class Object3D {
   public:
    Object3D();
    virtual ~Object3D();

    virtual void update();
    virtual void draw(glm::mat4 mat);
    virtual void drawShadow(glm::mat4 mat);
    virtual void onContact(const physx::PxRigidActor* actor, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
};

}  // namespace world