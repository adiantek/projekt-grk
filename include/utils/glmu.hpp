#pragma once

#include <assimp/matrix4x4.h>
#include <stdbool.h>
#include <glm/ext.hpp>

namespace utils {

class glmu {
   public:
    /**
     * @brief Convert assimp matrix to glm matrix
     * 
     * @param aAssimpMat matrix to convert
     * @return glm::mat4 
     */
    static glm::mat4 to_mat4(const aiMatrix4x4 &aAssimpMat);

    static glm::vec3 inverse_scale(glm::vec3 vec);

    static glm::vec3 s_random_offset(glm::vec3 amplitudes, float time);
    static glm::vec3 s_random_offset(float amplitude, float time);

    static glm::vec3 midpoint(glm::vec3 a, glm::vec3 b);

    static glm::vec2 circles_midpoint(glm::vec2 a, glm::vec2 b, float aRadius, float bRadius);
    static glm::vec3 circles_midpoint(glm::vec3 a, glm::vec3 b, float aRadius, float bRadius, glm::vec3 up);

    static glm::vec3 curve(glm::vec3 a, glm::vec3 b, float t);
    static bool isEmpty(glm::vec3 vec);

    /**
     * @brief Compute barycentric coordinates (u, v, w) for point p with respect to triangle (a, b, c)
     */
    static inline void barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c, float *u, float *v, float *w) {
        // based on: https://gamedev.stackexchange.com/a/23745
        glm::vec2 v0 = b - a;
        glm::vec2 v1 = c - a;
        glm::vec2 v2 = p - a;

        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;
        *v = (d11 * d20 - d01 * d21) / denom;
        *w = (d00 * d21 - d01 * d20) / denom;
        *u = 1.0f - *v - *w;
    }
};
}  // namespace utils