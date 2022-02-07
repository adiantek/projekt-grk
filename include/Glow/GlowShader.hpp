#pragma once

#include <opengl.h>
#include <glm/glm.hpp>

namespace Glow {
class GlowShader {
   public:
    GlowShader(GLuint w, GLuint h);
    ~GlowShader();
    void resize(GLuint w, GLuint h);
    void startFB();
    void stopFB();
    void draw(glm::mat4 mat);
    void clear();

   private:
     GLuint fb, texture, w, h, vao, vbo;
};

extern GlowShader* glow;
}  // namespace Glow