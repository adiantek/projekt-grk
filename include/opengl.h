#ifndef OPENGL_H
#define OPENGL_H

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <webgl/webgl2.h>
#define GLFW_INCLUDE_ES31
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#define CHECK_OPENGL_ERROR() { GLenum err = glGetError(); if (err) LOGE("opengl err: %d", err); }

#endif