#pragma once

#include <glm/ext.hpp>
#include <assimp/matrix4x4.h>

namespace glmu {

    /**
     * @brief Convert assimp matrix to glm matrix
     * 
     * @param aAssimpMat matrix to convert
     * @return glm::mat4 
     */
    glm::mat4 to_mat4(const aiMatrix4x4& aAssimpMat);

    /**
     * @brief Set the magnitude object
     * 
     * @param vec input vector
     * @param magnitude final magnitude
     * @return glm::vec3 
     */
    glm::vec3 set_magnitude(glm::vec3 vec, float magnitude);

    glm::vec3 inverse_scale(glm::vec3 vec);

    glm::vec3 s_random_offset(glm::vec3 amplitudes, float time);
    glm::vec3 s_random_offset(float amplitude, float time);

}