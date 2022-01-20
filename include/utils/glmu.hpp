#pragma once

#include <assimp/matrix4x4.h>

#include <glm/ext.hpp>

namespace utils {

    /**
     * @brief Convert assimp matrix to glm matrix
     * 
     * @param aAssimpMat matrix to convert
     * @return glm::mat4 
     */
    glm::mat4 to_mat4(const aiMatrix4x4& aAssimpMat);

    glm::vec3 inverse_scale(glm::vec3 vec);

    glm::vec3 s_random_offset(glm::vec3 amplitudes, float time);
    glm::vec3 s_random_offset(float amplitude, float time);

    glm::vec3 midpoint(glm::vec3 a, glm::vec3 b);

    glm::vec2 circles_midpoint(glm::vec2 a, glm::vec2 b, float aRadius, float bRadius);

    glm::vec3 curve(glm::vec3 a, glm::vec3 b, float t);

}  // namespace utils