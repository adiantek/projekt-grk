#include <cmath>
#include <glm/ext.hpp>
#include <assimp/matrix4x4.h>

#include <Utilities/glmu.hpp>

namespace glmu {

    glm::mat4 to_mat4(const aiMatrix4x4& aAssimpMat) {
        return glm::transpose(glm::make_mat4(&aAssimpMat.a1));
    }

    glm::vec3 set_magnitude(glm::vec3 vec, float magnitude) {
        return glm::normalize(vec) * magnitude;
    }

    glm::vec3 inverse_scale(glm::vec3 vec) {
        return glm::vec3(1.0f / vec.x, 1.0f / vec.y, 1.0f / vec.z);
    }

    glm::vec3 s_random_offset(float amplitude, float time) {
        return glmu::s_random_offset(glm::vec3(amplitude), time);
    }

    glm::vec3 s_random_offset(glm::vec3 amplitudes, float time) {
        return glm::vec3(
            (sin(time/31) + sin(time/27) + sin(time/13)) / 3 * amplitudes.x,
            (sin(time/17) + sin(time/19) + sin(time/11)) / 3 * amplitudes.y,
            (sin(time/23) + sin(time/29) + sin(time/31)) / 3 * amplitudes.z
        );
    }

}