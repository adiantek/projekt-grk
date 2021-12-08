#ifndef OPENGL_H
#define OPENGL_H

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#endif