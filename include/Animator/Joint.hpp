#pragma once

#include <glm/ext.hpp>
#include <list>

namespace Animator {
  
  class Joint {
    public:
      int index;
      std::string name;
      Joint* parent;
      std::list<Joint> children;

      Joint(int index, std::string name, glm::mat4 localBindTransform, Joint* parent, std::list<Joint> children);
      Joint(int index, std::string name, glm::mat4 localBindTransform);
      ~Joint();

      void addChild(Joint child);
      glm::mat4 getTransform();
      void setTransform(glm::mat4 transform);
      glm::mat4 getInverseBindTransform();
      void calculateInverseBindTransform(glm::mat4 parentBindTransform);
    private:
      glm::mat4 transform;
      glm::mat4 localBindTransform;
      glm::mat4 inverseBindTransform;
  };

}