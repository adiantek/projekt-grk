#pragma once

#include <Animator/Joint.hpp>
#include <vector>

namespace Animator {
class JointIO {
    public:
    static char *serialize(std::vector<Joint*> vector, size_t *outLen);
    static std::vector<Joint*> deserialize(char *data, size_t dataLen);
};
}  // namespace Animator