#include <assimp/matrix4x4.h>

#include <cmath>
#include <glm/ext.hpp>
#include <utils/glmu.hpp>

# define M_PI 3.14159265358979323846

namespace utils {

glm::mat4 glmu::to_mat4(const aiMatrix4x4& aAssimpMat) {
    return glm::transpose(glm::make_mat4(&aAssimpMat.a1));
}

glm::vec3 glmu::inverse_scale(glm::vec3 vec) {
    return glm::vec3(1.0f / vec.x, 1.0f / vec.y, 1.0f / vec.z);
}

glm::vec3 glmu::s_random_offset(float amplitude, float time) {
    return s_random_offset(glm::vec3(amplitude), time);
}

glm::vec3 glmu::s_random_offset(glm::vec3 amplitudes, float time) {
    return glm::vec3(
        (sin(time / 31) + sin(time / 27) + sin(time / 13)) / 3 * amplitudes.x,
        (sin(time / 17) + sin(time / 19) + sin(time / 11)) / 3 * amplitudes.y,
        (sin(time / 23) + sin(time / 29) + sin(time / 31)) / 3 * amplitudes.z);
}

glm::vec3 glmu::midpoint(glm::vec3 a, glm::vec3 b) {
    return glm::vec3(
        (a.x + b.x) / 2,
        (a.y + b.y) / 2,
        (a.z + b.z) / 2);
}

glm::vec2 glmu::circles_midpoint(glm::vec2 a, glm::vec2 b, float aRadius, float bRadius) {
    float r = aRadius;
    float R = bRadius;
    float x1 = a.x;
    float y1 = a.y;
    float x2 = b.x;
    float y2 = b.y;

    float dl = (float)sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    float cosA = (dl * dl + r * r - R * R) / (2 * dl * r);
    float sinA = (float)sqrt(1 - pow(cosA, 2));
    float vpx = (x2 - x1) * r / dl;
    float vpy = (y2 - y1) * r / dl;

    float V1x = vpx * cosA - vpy * sinA + x1;
    float V1y = vpx * sinA + vpy * cosA + y1;

    float V2x = vpx * cosA + vpy * sinA + x1;
    float V2y = vpy * cosA - vpx * sinA + y1;

    if (V2y > V1y) {
        return glm::vec2(V2x, V2y);
    } else {
        return glm::vec2(V1x, V1y);
    }
}

glm::vec3 glmu::circles_midpoint(glm::vec3 a, glm::vec3 b, float aRadius, float bRadius, glm::vec3 up) {
    float d = glm::distance(a, b);

    if (aRadius + bRadius <= d) {
        return (glm::normalize(a - b) * bRadius) + b;
    }

    float h = (1.0f / 2.0f) + ((aRadius * aRadius) - (bRadius * bRadius)) / (2 * d * d);
    glm::vec3 c_i = a + (h * (b - a));
    float r_i = sqrt((aRadius * aRadius) - (h * h * d * d));
    glm::vec3 n_i = (b - a) / d;

    glm::vec3 t_i = glm::normalize(glm::cross(up, n_i));
    glm::vec3 b_i = glm::cross(t_i, n_i);

    return c_i + r_i * (t_i * cos(M_PI + M_PI/2) + b_i * sin(M_PI + M_PI/2));
}

glm::vec3 glmu::curve(glm::vec3 a, glm::vec3 b, float t) {
    t = std::min(1.0f, std::max(0.0f, t));
    glm::vec3 resultVec = a + (b - a) * t;
    resultVec.z += (float)sin(t * M_PI) * 0.2f;
    return resultVec;
}

}  // namespace utils