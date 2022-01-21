#pragma once

#include <vertex/VertexBuffer.hpp>
#include <glm/glm.hpp>
#include <stdbool.h>

namespace utils {

class Line {
   private:
    Line();
    static inline bool initialized = false;
    static GLuint vao;
    static void init();

   public:
     static void draw(glm::mat4 mat, glm::vec3 start, glm::vec3 end, glm::vec3 color);
};

}  // namespace utils