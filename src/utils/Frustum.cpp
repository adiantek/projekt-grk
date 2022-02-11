#include <utils/Frustum.hpp>

using namespace utils;

Frustum::Frustum() {
}

void Frustum::loadPlanes(glm::mat4 mat) {
    for (int i = 0; i < 4; i++) {
        this->frustum[0][i] = mat[i][3] - mat[i][0];
        this->frustum[1][i] = mat[i][3] + mat[i][0];
        this->frustum[2][i] = mat[i][3] + mat[i][1];
        this->frustum[3][i] = mat[i][3] - mat[i][1];
        this->frustum[4][i] = mat[i][3] - mat[i][2];
        this->frustum[5][i] = mat[i][3] + mat[i][2];
    }
    for (int i = 0; i < 6; i++) {
        this->frustum[i] = glm::normalize(this->frustum[i]);
    }
}

bool Frustum::isBoxInFrustum(float sx, float sy, float sz, float ex, float ey, float ez) {
    for (int i = 0; i < 6; i++) {
        glm::vec4 vec = this->frustum[i];
        float x = vec[0];
        float y = vec[1];
        float z = vec[2];
        float w = vec[3];

        if (x * sx + y * sy + z * sz + w <= 0.0F &&
            x * ex + y * sy + z * sz + w <= 0.0F &&
            x * sx + y * ey + z * sz + w <= 0.0F &&
            x * ex + y * ey + z * sz + w <= 0.0F &&
            x * sx + y * sy + z * ez + w <= 0.0F &&
            x * ex + y * sy + z * ez + w <= 0.0F &&
            x * sx + y * ey + z * ez + w <= 0.0F &&
            x * ex + y * ey + z * ez + w <= 0.0F) {
            return false;
        }
    }

    return true;
}