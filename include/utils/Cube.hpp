#pragma once

#include <vertex/VertexBuffer.hpp>
#include <glm/glm.hpp>
#include <stdbool.h>

namespace utils {

class Cube {
   private:
    Cube();
    static inline bool initialized = false;
    static GLuint vao;
    static void init();

   public:
     static void draw(glm::mat4 mat, glm::vec3 position, glm::vec3 size, glm::vec3 color, bool diagonals);
};

}  // namespace utils