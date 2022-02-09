#pragma once

#include <opengl.h>
#include <stdbool.h>
#include <glm/glm.hpp>

namespace utils {
class Frustum {
   private:
    glm::vec4 frustum[6];

   public:
    Frustum();
    void loadPlanes(glm::mat4 mat);
    bool isBoxInFrustum(float f, float f1, float f2, float f3, float f4, float f5);
};
}  // namespace utils
