#pragma once

#include <stdbool.h>

namespace utils {
class Compression {
   public:
    static bool compress(const char *filename, char *data, size_t dataLen);
    static char *decompress(const char *filename, size_t *outDataLen);
};

}  // namespace utils
