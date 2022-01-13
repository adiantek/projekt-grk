#pragma once

#include <opengl.h>
#include <Animator/Joint.hpp>

namespace Animator {

  class AnimatedModel {
    public:
      AnimatedModel(GLuint model, GLuint texture, Joint* rootJoint, int jointCount);
      ~AnimatedModel();

      GLuint getModel();
      GLuint getTexture();
      Joint* getRootJoint();
      void destroy();
      void doAnimation();
      void Update();
      glm::mat4* getJointTransform();
      void addJointsToArray(Joint* headJoint, glm::mat4 jointMatrices[]);
    private:
      GLuint model;
      GLuint texture;
      Joint* rootJoint;
      int jointCount;
  };

}