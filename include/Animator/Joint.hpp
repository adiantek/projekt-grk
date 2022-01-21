#pragma once

#include <glm/ext.hpp>
#include <vector>
#include <stdbool.h>

#define JOINT_MAX_LENGTH 64

namespace Animator {
  class Joint {
    public:
      /* Index of the joint (same as index in the mesh joints array) */
      int index;
      /* Parent joint */
      Joint* parent;
      /* Children joints */
      std::vector<Joint*> children;

      Joint();
      Joint(int index, std::string name, glm::mat4 transform, Joint* parent, std::vector<Joint*> children);
      Joint(int index, std::string name, glm::mat4 transform, std::vector<Joint*> children);
      Joint(int index, std::string name, glm::mat4 transform, Joint* parent);
      Joint(int index, std::string name, glm::mat4 transform);
      ~Joint();

      void calculateInverseBindTransform(glm::mat4 parentBindTransform);

      Joint* setTransform(glm::mat4 transform);

      Joint* setLocalBindTransform(glm::mat4 localBindTransform);
      Joint* setParent(Joint* parent);
      Joint* addChild(Joint* child);
      
      glm::mat4 getTransform();
      /* Matrix that transforms from mesh space to bone space. */
      glm::mat4 getInverseBindTransform();

      /* Matrix that transforms from bone space to mesh space in bind pose.
        This matrix describes the position of the mesh
        in the local space of this bone when the skeleton was bound.
        Thus it can be used directly to determine a desired vertex position,
        given the world-space transform of the bone when animated,
        and the position of the vertex in mesh space. */
      glm::mat4 getLocalBindTransform();

      glm::vec3 getOrigin();

      /* Returns true if the joint has parent */
      bool hasParent();
      /* Returns true if the joint has children */
      bool hasChildren();
      /* Name of the joint */
      std::string getName();
    private:
      /* Name of the joint */
      std::string name;
      bool _hasParent = false;
      /* Tranformation of the joint relative to it's parent */
      glm::mat4 transform;
      /* Matrix that transforms from bone space to mesh space in bind pose.
        This matrix describes the position of the mesh
        in the local space of this bone when the skeleton was bound.
        Thus it can be used directly to determine a desired vertex position,
        given the world-space transform of the bone when animated,
        and the position of the vertex in mesh space. */
      glm::mat4 localBindTransform;

      /* Matrix that transforms from mesh space to bone space. */
      glm::mat4 inverseBindTransform;
  };

}